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

#include <cstring>
#include <algorithm>

#include "abstractaction.h"

#include "cardtools.h"
#include "smartptr.h"
#include "iaistate.h"

namespace {
NetMauMau::AIDT::IConditionPtr NULLCONDITION;

const NetMauMau::Common::ICard::SUIT SUIT[4] = {
	NetMauMau::Common::ICard::HEARTS,
	NetMauMau::Common::ICard::DIAMONDS,
	NetMauMau::Common::ICard::SPADES,
	NetMauMau::Common::ICard::CLUBS
};

}

using namespace NetMauMau::AIDT;

AbstractAction::AbstractAction() : IAction() {}

AbstractAction::~AbstractAction() {}

void AbstractAction::countSuits(SUITCOUNT *suitCount,
								const NetMauMau::Player::IPlayer::CARDS &myCards) const {

	std::memset(suitCount, 0, sizeof(SUITCOUNT) * 4);

	const bool noCards = myCards.empty();

	for(std::size_t i = 0; i < 4; ++i) {
		const SUITCOUNT sc = { SUIT[i], noCards ? 0 : std::count_if(myCards.begin(), myCards.end(),
							   std::bind2nd(std::ptr_fun(NetMauMau::Common::isSuit), SUIT[i]))
							 };
		suitCount[i] = sc;
	}

	if(!noCards) std::sort(suitCount, suitCount + 4);
}

const IConditionPtr &AbstractAction::getNullCondition() {
	return NULLCONDITION;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
