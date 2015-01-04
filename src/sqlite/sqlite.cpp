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

#include "sqlite.h"
#include "sqliteimpl.h"

using namespace NetMauMau::DB;

SQLite::SQLite() : _pimpl(new SQLiteImpl()) {}

SQLite::~SQLite() {
	delete _pimpl;
}

SQLite &SQLite::getInstance() {
	static SQLite instance;
	return instance;
}

bool SQLite::addPlayer(const NetMauMau::Common::AbstractConnection::INFO &info) const {
	return _pimpl->addPlayer(info);
}

bool SQLite::logOutPlayer(const NetMauMau::Common::AbstractConnection::NAMESOCKFD &nsf) const {
	return _pimpl->logOutPlayer(nsf);
}

long long int SQLite::newGame() const {
	return _pimpl->newGame();
}

bool SQLite::gameEnded(long long int gameIndex) const {
	return _pimpl->gameEnded(gameIndex);
}

bool SQLite::addPlayerToGame(long long int gid,
							 const NetMauMau::Common::AbstractConnection::NAMESOCKFD &nsf) const {
	return _pimpl->addPlayerToGame(gid, nsf);
}

bool SQLite::turn(long long int gameIndex, std::size_t t) const {
	return _pimpl->turn(gameIndex, t);
}

bool SQLite::gamePlayStarted(long long int gameIndex) const {
	return _pimpl->gamePlayStarted(gameIndex);
}

bool SQLite::playerLost(long long int gameIndex,
						const NetMauMau::Common::AbstractConnection::NAMESOCKFD &nsf,
						time_t time, std::size_t points) const {
	return _pimpl->playerLost(gameIndex, nsf, time, points);
}

bool SQLite::playerWins(long long int gameIndex,
						const NetMauMau::Common::AbstractConnection::NAMESOCKFD &nsf) const {
	return _pimpl->playerWins(gameIndex, nsf);
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
