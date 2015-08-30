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

#ifndef NETMAUMAU_DB_SQLITE_H
#define NETMAUMAU_DB_SQLITE_H

#include <cstddef>                      // for size_t
#include <ctime>                        // for time_t

#include "iconnection.h"                // for IConnection, etc
#include "smartsingleton.h"

#define NOGAME_IDX -1LL

namespace NetMauMau {

namespace Player {
class IPlayer;
}

namespace DB {

typedef long long int GAMEIDX;

class SQLiteImpl;

class SQLite : public Common::SmartSingleton<SQLite> {
	DISALLOW_COPY_AND_ASSIGN(SQLite)
	friend class Common::SmartSingleton<SQLite>;
public:
	typedef smartptr_type SQLitePtr;

	typedef struct {
		GAMEIDX id;
		std::string name;
		long long int score;
	} SCORE;

	typedef std::vector<SCORE> SCORES;
	typedef enum { NORM, ABS } SCORE_TYPE;

	virtual ~SQLite() throw();

	static std::string getDBFilename();

	SCORES getScores(SCORE_TYPE type) const;
	SCORES getScores(SCORE_TYPE type, std::size_t limit) const;

	long long int getServedGames() const;

	bool addAIPlayer(const NetMauMau::Player::IPlayer *ai) const;
	bool addPlayer(const Common::IConnection::INFO &info) const;
	bool logOutPlayer(const Common::IConnection::NAMESOCKFD &nsf) const;
	GAMEIDX newGame() const;
	bool gameEnded(GAMEIDX gameIndex) const;
	bool addPlayerToGame(GAMEIDX gid, const Common::IConnection::NAMESOCKFD &nsf) const;
	bool turn(GAMEIDX gameIndex, std::size_t turn) const;
	bool gamePlayStarted(GAMEIDX gameIndex) const;
	bool playerLost(GAMEIDX gameIndex, const Common::IConnection::NAMESOCKFD &nsf,
					std::time_t time, std::size_t points) const;
	bool playerWins(GAMEIDX gameIndex, const Common::IConnection::NAMESOCKFD &nsf) const;

private:
	explicit SQLite();

private:
	// cppcheck-suppress unsafeClassCanLeak
	SQLiteImpl *const _pimpl;
};

}

}

#endif /* NETMAUMAU_DB_SQLITE_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
