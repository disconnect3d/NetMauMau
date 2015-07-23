/*
 * Copyright 2015 by Heiko Schäfer <heiko@rangun.de>
 *
 * This file is part of NetMauMau.
 *
 * NetMauMau is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * NetMauMau is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with NetMauMau.  If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(HAVE_CONFIG_H) || defined(IN_IDE_PARSER)
#include "config.h"
#endif

#if defined(IN_IDE_PARSER)
#define PKGDATADIR ""
#endif

#include <fstream>
#include <cstring>
#include <cstdarg>
#include <iterator>

#include <stdint.h>
#include <sys/types.h>

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifndef _WIN32
#define MHD_PLATFORM_H 1
#endif

#include <microhttpd.h>

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#include "httpd.h"
#include "base64.h"
#include "logger.h"
#include "helpers.h"
#include "iplayer.h"
#include "defaultplayerimage.h"
#include "abstractsocketimpl.h"

#ifndef _WIN32
#define TIMEFORMAT "%T - "
#else
#define TIMEFORMAT "%H:%M:%S - "
#endif

namespace {

const char *B2TOP = "<a href=\"#top\">Back to top</a>";

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic push
struct scoresTable : std::unary_function<NetMauMau::DB::SQLite::SCORES::value_type, void> {
	inline explicit scoresTable(std::ostringstream &o) : os(o), pos(0) {}
	inline result_type operator()(const argument_type &s) const {
		os << "<tr><td align=\"right\">" << ++pos << "&nbsp;</td><td>&nbsp;" << s.name
		   << "</td><td align=\"center\">" << (s.score < 0 ? "<font color=\"red\">" : "") << s.score
		   << (s.score < 0 ? "</font>" : "") << "</td></tr>";
	}

private:
	std::ostringstream &os;
	mutable std::size_t pos;
};

struct capaTable :
		std::unary_function<NetMauMau::Common::AbstractConnection::CAPABILITIES::value_type, void> {
	inline explicit capaTable(std::ostringstream &o) : os(o) {}
	inline result_type operator()(const argument_type &p) const {
		const bool isWurl = p.first == "WEBSERVER_URL";
		os << "<tr><td>&nbsp;" << p.first << "</td><td>&nbsp;" << (isWurl ? "<a href=\"" : "")
		   << p.second << (isWurl ? "\">" : "") << (isWurl ? p.second : "")
		   << (isWurl ? "</a>" : "") << "</td></tr>";
	}

private:
	std::ostringstream &os;
};

struct listPlayers : std::unary_function<NetMauMau::Server::Httpd::PLAYERS::value_type, void> {
	inline explicit listPlayers(std::ostringstream &o) : os(o) {}
	inline result_type operator()(const argument_type &p) const {
		os << "<li><a href=\"/images/" << p->getName() << "\"><img height=\"30\" src=\"/images/"
		   << p->getName() << "\">" << "</a>&nbsp;<b>" << p->getName() << "</b>&nbsp;<i>("
		   << (p->getType() == NetMauMau::Player::IPlayer::HUMAN ? "human player" :
			   (p->getType() == NetMauMau::Player::IPlayer::HARD ? "hard AI" : "easy AI"))
		   << ")</i></li>";
	}

private:
	std::ostringstream &os;
};
#pragma GCC diagnostic pop

int answer_to_connection(void *cls, struct MHD_Connection *connection, const char *url,
						 const char */*method*/, const char */*version*/,
						 const char */*upload_data*/,
#if MHD_VERSION > 0x00000200
						 size_t */*upload_data_size*/,
#else
						 unsigned int */*upload_data_size*/,
#endif
						 void **/*con_cls*/) {

	const NetMauMau::Server::Httpd *httpd = reinterpret_cast<NetMauMau::Server::Httpd *>(cls);
	const bool havePlayers = !httpd->getPlayers().empty();

	std::vector<std::string::traits_type::char_type> bin;
	std::ostringstream os;
	os.unsetf(std::ios_base::skipws);

	const char *contentType = 0L;
	bool binary = false;

	if(!std::strncmp("/images/", url, 8)) {

		binary = true;
		contentType = "image/png";
		bin.clear();

		const char *name = std::strrchr(url, '/');

		if(name && *(name + 1)) {

			const NetMauMau::Server::Httpd::IMAGES::const_iterator
			&f(httpd->getImages().find(name + 1));

			if(f != httpd->getImages().end()) {
				bin.insert(bin.end(), f->second.begin(), f->second.end());
			} else {
				bin.insert(bin.end(), NetMauMau::Common::DefaultPlayerImage.begin(),
						   NetMauMau::Common::DefaultPlayerImage.end());
			}

		} else {
			bin.insert(bin.end(), NetMauMau::Common::DefaultPlayerImage.begin(),
					   NetMauMau::Common::DefaultPlayerImage.end());
		}

	} else if(!std::strncmp("/favicon.ico", url, 8)) {

		binary = true;
		contentType = "image/vnd.microsoft.icon";

		std::ifstream fav(PKGDATADIR "/netmaumau.ico", std::ios::binary);

		bin.clear();

		if(!fav.fail()) {
			bin.insert(bin.end(),
					   std::istreambuf_iterator<std::string::traits_type::char_type>(fav),
					   std::istreambuf_iterator<std::string::traits_type::char_type>());
		} else {
			logWarning("Failed to open favicon file: \"" << PKGDATADIR << "/netmaumau.ico\"");
		}

	} else {

		contentType = "text/html; charset=utf-8";

		const NetMauMau::DB::SQLite::SCORES &sc(httpd->getCapabilities().find("HAVE_SCORES") !=
												httpd->getCapabilities().end() ?
												NetMauMau::DB::SQLite::getInstance()->
												getScores(NetMauMau::DB::SQLite::NORM) :
												NetMauMau::DB::SQLite::SCORES());

		os << "<html><head>"
		   << "<link rel=\"shortcut icon\" type=\"image/vnd.microsoft.icon\" "
		   << "href=\"/favicon.ico\" />"
		   << "<link rel=\"icon\" type=\"image/vnd.microsoft.icon\" href=\"/favicon.ico\" />"
		   << "<title>" << PACKAGE_STRING << " (" << BUILD_TARGET << ")</title>";

		os << "<style>"
		   << "table, td, th { background-color:white; border: thin solid black; "
		   << "border-spacing: 0; border-collapse: collapse; }"
		   << "pre { background-color:white; }"
		   << "a { text-decoration:none; }"
		   << "img { border:none; }"
		   << "</style></head>";

		os << "<body bgcolor=\"#eeeeee\"><a name=\"top\"><font face=\"Sans-Serif\">"
		   << "<h1 align=\"center\">" << PACKAGE_STRING << "</h1></a><hr />";

		os << "<p><ul>";

		if(havePlayers) os << "<li><a href=\"#players\">Players online</a></li>";

		if(!sc.empty()) os << "<li><a href=\"#scores\">Hall of Fame</a></li>";

		os << "<li><a href=\"#capa\">Server capabilities</a></li>";
		os << "<li><a href=\"#dump\">Server dump</a></li>";

		os << "</ul></p><hr />";

		if(havePlayers) {
			os << "<a name=\"players\"><h2 align=\"center\">Players online <i>("
			   << (httpd->isWaiting() ?  "waiting" : "running") << ")</i></h2><p><ol>";

			std::for_each(httpd->getPlayers().begin(), httpd->getPlayers().end(), listPlayers(os));

			os << "</ol></p></a>" << B2TOP << "<hr />";
		}

		if(!sc.empty()) {
			os << "<a name=\"scores\"><center><h2>Hall of Fame</h2><table width=\"50%\">"
			   << "<tr><th>&nbsp;</th><th>PLAYER</th><th>SCORE</th></tr>";

			std::for_each(sc.begin(), sc.end(), scoresTable(os));

			os << "</table></center></a>" << B2TOP << "<hr />";
		}

		os << "<a name=\"capa\"><center><h2>Server capabilities</h2><table width=\"50%\">"
		   << "<tr><th>NAME</th><th>VALUE</th></tr>";

		std::for_each(httpd->getCapabilities().begin(), httpd->getCapabilities().end(),
					  capaTable(os));

		os << "</table></center></a>" << B2TOP << "<hr /><a name=\"dump\">"
		   << "<h2 align=\"center\">Server dump</h2><tt><pre>";

		NetMauMau::dump(os);

		os << "</pre></a></tt><hr />" << B2TOP << "</font></body></html>";
	}

	void *data = 0L;

	if(binary) {

		if((data = malloc(bin.size()))) {
			std::memcpy(data, bin.data(), bin.size());
		}

	} else {
		data = static_cast<void *>(const_cast<char *>(strndup(os.str().c_str(),
								   os.str().length())));
	}

	const std::size_t len = binary ? (data ? bin.size() : 0u) : os.str().length();

	bin.clear();

	MHD_Response *response = MHD_create_response_from_data(len, data, true, false);

	MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, contentType);
	MHD_add_response_header(response, MHD_HTTP_HEADER_EXPIRES, "Thu, 01 Dec 1994 16:00:00 GMT");
	MHD_add_response_header(response, MHD_HTTP_HEADER_CACHE_CONTROL, "no-cache");

	const int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);

	MHD_destroy_response(response);

	return ret;
}

}

using namespace NetMauMau::Server;

HttpdPtr Httpd::m_instance;

Httpd::Httpd() : m_daemon(0L), m_gameSource(0L), m_engineSource(0L), m_connectionSource(0L),
	m_players(), m_images(), m_caps(), m_gameRunning(false), m_waiting(true), m_url() {

	struct addrinfo *ai = NULL;

	if(NetMauMau::httpd && !NetMauMau::Common::AbstractSocketImpl::getAddrInfo(NetMauMau::host &&
			*NetMauMau::host ? NetMauMau::host : 0L, static_cast<uint16_t>(NetMauMau::hport), &ai,
			true) && ai && !(m_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
										static_cast<unsigned short>(NetMauMau::hport), NULL, NULL,
										&answer_to_connection, this,
										MHD_OPTION_CONNECTION_LIMIT, 10,
#if MHD_VERSION > 0x00000200
										MHD_OPTION_SOCK_ADDR, ai->ai_addr,
										MHD_OPTION_PER_IP_CONNECTION_LIMIT, 10,
										MHD_OPTION_THREAD_POOL_SIZE, 5,
#endif
										MHD_OPTION_END))) {

		logWarning(NetMauMau::Common::Logger::time(TIMEFORMAT)
				   << "Failed to start webserver at http://"
#if MHD_VERSION > 0x00000200
				   << (ai && ai->ai_canonname ? ai->ai_canonname : "localhost")
#else
				   << "localhost"
#endif
				   << ":" << NetMauMau::hport);

	} else if(NetMauMau::httpd && ai && m_daemon) {
		logInfo(NetMauMau::Common::Logger::time(TIMEFORMAT) << "Started webserver at http://"
#if MHD_VERSION > 0x00000200
				<< (ai && ai->ai_canonname ? ai->ai_canonname : "localhost")
#else
				<< "localhost"
#endif
				<< ":" << NetMauMau::hport);

		std::ostringstream uos;

		uos << "http://"
#if MHD_VERSION > 0x00000200
			<< (ai && ai->ai_canonname ? ai->ai_canonname : "localhost") << ':'
#else
			<< "localhost:"
#endif
			<< NetMauMau::hport;

		m_url = uos.str();
	}

	freeaddrinfo(ai);
}

Httpd::~Httpd() {
	if(m_daemon) MHD_stop_daemon(m_daemon);
}

Httpd *Httpd::getInstance() {

	if(!m_instance) m_instance = HttpdPtr(new Httpd());

	return m_instance;
}

void Httpd::setSource(const NetMauMau::Common::IObserver<Connection>::source_type *s) {
	m_connectionSource = s;
}

void Httpd::setSource(const NetMauMau::Common::IObserver<NetMauMau::Engine>::source_type *s) {
	m_engineSource = s;
}

void Httpd::setSource(const NetMauMau::Common::IObserver<Game>::source_type *s) {
	m_gameSource = s;
}

void Httpd::update(const NetMauMau::Common::IObserver<Connection>::what_type &what) {

	const std::vector<NetMauMau::Common::BYTE> &b64(NetMauMau::Common::base64_decode(what.second));

	if(b64.empty()) {
		m_images.insert(std::make_pair(what.first, NetMauMau::Common::DefaultPlayerImage));
	} else {
		m_images.insert(std::make_pair(what.first, std::string(b64.begin(), b64.end())));
	}
}

void Httpd::update(const NetMauMau::Common::IObserver<NetMauMau::Engine>::what_type &what) {
	m_players = what;
}

void Httpd::update(const NetMauMau::Common::IObserver<Game>::what_type &what) {

	switch(what) {
	case PLAYERADDED:
	case PLAYERREMOVED:
		break;

	case READY:
		m_waiting = false;
		break;

	case GAMESTARTED:
		m_gameRunning = true;
		m_waiting = false;
		break;

	case GAMEENDED:
		m_players.clear();
		m_images.clear();
		m_waiting = true;
		m_gameRunning = false;
		break;
	}
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
