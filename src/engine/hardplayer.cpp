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

#include "hardplayer.h"

#include "jackonlycondition.h"          // for JackOnlyCondition
#include "powerjackcondition.h"         // for PowerJackCondition

using namespace NetMauMau::Player;

HardPlayer::HardPlayer(const std::string &name, const NetMauMau::IPlayedOutCards *poc)
	: AIPlayerBase < NetMauMau::AI::JackOnlyCondition,
	  NetMauMau::AI::PowerJackCondition > (name, poc) {}

HardPlayer::~HardPlayer() throw() {}

IPlayer::TYPE HardPlayer::getType() const throw() {
	return HARD;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
