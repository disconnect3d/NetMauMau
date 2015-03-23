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

#if defined(HAVE_CONFIG_H) || defined(IN_IDE_PARSER)
#include "config.h"
#endif

#include <ctime>
#include <cstdlib>

#include "logger.h"
#include "engine.h"
#include "stdplayer.h"
#include "engineconfig.h"
#include "testeventhandler.h"

using namespace NetMauMau;

int main(int, char **) {

#ifndef HAVE_ARC4RANDOM_UNIFORM
	std::srand(static_cast<unsigned int>(std::time(0L)));
#endif

	try {
		TestEventHandler evHdlr;
		EngineConfig cfg(evHdlr, 0L, false, 'A', true);
// 	EngineConfig cfg(evHdlr, true, 0L, false, 'A');
		Engine engine(cfg);

		Player::StdPlayer p1("Cathy");
		Player::StdPlayer p2("Tarik");
		Player::StdPlayer p3("Alischa");
		Player::StdPlayer p4("Heiko");

		engine.addPlayer(&p1);
		engine.addPlayer(&p2);
		engine.addPlayer(&p3);
		engine.addPlayer(&p4);

		engine.distributeCards();

		while(engine.hasPlayers()) {
			engine.nextTurn();
		}

	} catch(const NetMauMau::Common::Exception::SocketException &e) {
		logError(e);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
