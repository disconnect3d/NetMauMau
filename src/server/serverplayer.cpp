/*
 * Copyright 2014-2015 by Heiko Schäfer <heiko@rangun.de>
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

#include "serverplayer.h"

#include <cstdlib>                      // for strtoul
#include <cstdio>                       // for snprintf, NULL

#include "cardtools.h"                  // for getIllegalCard, etc
#include "serverconnection.h"           // for Connection
#include "serverplayerexception.h"      // for ServerPlayerException

#if defined(_WIN32)
#undef TRUE
#undef FALSE
#undef ERROR
#endif

#include "protocol.h"                   // for ACEROUND, CARDACCEPTED, etc

namespace {
const std::string ILLEGAL_CARD(NetMauMau::Common::getIllegalCard()->description());
}

using namespace NetMauMau::Server;

Player::Player(const std::string &name, int sockfd, Connection &con) : AbstractPlayer(name, 0L),
	m_connection(con), m_sockfd(sockfd) {}

Player::~Player() {}

int Player::getSerial() const {
	return m_sockfd;
}

bool Player::isAIPlayer() const {
	return false;
}

Player::IPlayer::TYPE Player::getType() const {
	return HUMAN;
}

bool Player::isAlive() const {

	try {
		NetMauMau::Common::AbstractSocket::checkSocket(m_sockfd);
	} catch(const Exception::ServerPlayerException &) {
		return false;
	}

	return true;
}

void Player::receiveCard(const NetMauMau::Common::ICardPtr &card) {

	try {
		if(card) receiveCardSet(CARDS(1, card));
	} catch(const NetMauMau::Common::Exception::SocketException &) {
		throw Exception::ServerPlayerException(__FUNCTION__);
	}
}

void Player::receiveCardSet(const CARDS &cards)
throw(NetMauMau::Common::Exception::SocketException) {

	NetMauMau::Player::AbstractPlayer::receiveCardSet(cards);

	try {
		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::GETCARDS);

		for(CARDS::const_iterator i(cards.begin()); i != cards.end(); ++i) {
			m_connection.write(m_sockfd, (*i)->description());
		}

		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::CARDSGOT);

	} catch(const NetMauMau::Common::Exception::SocketException &) {
		throw Exception::ServerPlayerException(__FUNCTION__);
	}
}

void Player::shuffleCards() {}

NetMauMau::Common::ICardPtr Player::requestCard(const NetMauMau::Common::ICardPtr &uncoveredCard,
		const NetMauMau::Common::ICard::SUIT *s, std::size_t takeCount, bool) const {

	try {

		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::OPENCARD);
		m_connection.write(m_sockfd, uncoveredCard->description());;
		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::PLAYCARD);

		const CARDS &posCards(getPossibleCards(uncoveredCard, s));

		for(CARDS::const_iterator i(posCards.begin()); i != posCards.end(); ++i) {
			m_connection.write(m_sockfd, (*i)->description());
		}

		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::PLAYCARDEND);

		if(getClientVersion() >= 8) {

			char cc[20];

#ifndef _WIN32
			std::snprintf(cc, 20, "%zu", takeCount);
#else
			std::snprintf(cc, 20, "%lu", (unsigned long)takeCount);
#endif

			m_connection.write(m_sockfd, cc);
		}

		const std::string offeredCard = m_connection.read(m_sockfd);

		if(offeredCard == NetMauMau::Common::Protocol::V15::SUSPEND) {
			return NetMauMau::Common::ICardPtr();
		} else if(offeredCard == ILLEGAL_CARD) {
			return NetMauMau::Common::ICardPtr(const_cast<const NetMauMau::Common::ICard *>
											   (NetMauMau::Common::getIllegalCard()));
		}

		return findCard(offeredCard);

	} catch(const NetMauMau::Common::Exception::SocketException &) {
		throw Exception::ServerPlayerException(__FUNCTION__);
	}
}

NetMauMau::Common::ICardPtr Player::findCard(const std::string &offeredCard) const {

	NetMauMau::Common::ICard::SUIT s = NetMauMau::Common::ICard::HEARTS;
	NetMauMau::Common::ICard::RANK r = NetMauMau::Common::ICard::ACE;

	if(NetMauMau::Common::parseCardDesc(offeredCard, &s, &r)) {

		const CARDS &pc(getPlayerCards());

		for(CARDS::const_iterator i(pc.begin()); i != pc.end(); ++i) {
			if((*i)->getSuit() == s && (*i)->getRank() == r) return NetMauMau::Common::ICardPtr(*i);
		}
	}

	return NetMauMau::Common::ICardPtr();
}

bool Player::cardAccepted(const NetMauMau::Common::ICard *playedCard)
throw(NetMauMau::Common::Exception::SocketException) {

	NetMauMau::Player::AbstractPlayer::cardAccepted(playedCard);

	try {

		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::CARDACCEPTED);
		m_connection.write(m_sockfd, playedCard->description());

		return !getCardCount();

	} catch(const NetMauMau::Common::Exception::SocketException &) {
		throw Exception::ServerPlayerException(__FUNCTION__);
	}
}

void Player::talonShuffled() throw(NetMauMau::Common::Exception::SocketException) {
	NetMauMau::Player::AbstractPlayer::talonShuffled();
	m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::TALONSHUFFLED);
}

Player::IPlayer::REASON Player::getNoCardReason(const NetMauMau::Common::ICardPtr &uncoveredCard,
		const NetMauMau::Common::ICard::SUIT *suit) const {

	if(getClientVersion() >= 15) {
		return !getPossibleCards(uncoveredCard, suit).empty() ? SUSPEND :
			   AbstractPlayer::getNoCardReason(uncoveredCard, suit);
	} else {
		return SUSPEND;
	}
}

std::size_t Player::getCardCount() const throw(NetMauMau::Common::Exception::SocketException) {

	std::size_t cc = 0;

	try {
		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::CARDCOUNT);
		cc = std::strtoul(m_connection.read(m_sockfd).c_str(), NULL, 10);
	} catch(const NetMauMau::Common::Exception::SocketException &) {
		throw Exception::ServerPlayerException(__FUNCTION__);
	}

	return cc;
}

NetMauMau::Common::ICard::SUIT Player::getJackChoice(const NetMauMau::Common::ICardPtr &,
		const NetMauMau::Common::ICardPtr &) const
throw(NetMauMau::Common::Exception::SocketException) {

	try {
		m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::JACKCHOICE);
		return NetMauMau::Common::symbolToSuit(m_connection.read(m_sockfd));
	} catch(const NetMauMau::Common::Exception::SocketException &) {
		throw Exception::ServerPlayerException(__FUNCTION__);
	}
}

bool Player::getAceRoundChoice() const throw(NetMauMau::Common::Exception::SocketException) {

	if(isAceRoundAllowed()) {
		try {
			m_connection.write(m_sockfd, NetMauMau::Common::Protocol::V15::ACEROUND);
			return m_connection.read(m_sockfd) == NetMauMau::Common::Protocol::V15::TRUE;
		} catch(const NetMauMau::Common::Exception::SocketException &) {
			throw Exception::ServerPlayerException(__FUNCTION__);
		}
	}

	return false;
}

uint32_t Player::getClientVersion() const {
	return m_connection.getPlayerInfo(getName()).clientVersion;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
