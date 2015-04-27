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

#include "powersuitaction.h"

#include "powersuitcondition.h"
#include "aceroundcondition.h"
#include "cardtools.h"
#include "iaistate.h"

namespace {
NetMauMau::AIDT::IConditionPtr ACEREOUNDCOND(new NetMauMau::AIDT::AceRoundCondition());
NetMauMau::AIDT::IConditionPtr POWERSUITCOND(new NetMauMau::AIDT::PowerSuitCondition());
}

using namespace NetMauMau::AIDT;

PowerSuitAction::PowerSuitAction() : AbstractAction(), m_determineSuit(true),
	m_suit(NetMauMau::Common::ICard::SUIT_ILLEGAL) {}

PowerSuitAction::PowerSuitAction(NetMauMau::Common::ICard::SUIT suit) : AbstractAction(),
	m_determineSuit(false), m_suit(suit) {}

PowerSuitAction::~PowerSuitAction() {}

const IConditionPtr &PowerSuitAction::perform(IAIState &state,
		const NetMauMau::Player::IPlayer::CARDS &) const {

	if(m_determineSuit) {

		NetMauMau::Player::IPlayer::CARDS myCards(state.getPlayerCards());

		SUITCOUNT suitCount[4];
		AbstractAction::countSuits(suitCount, myCards);

		if(std::count_if(myCards.begin(), myCards.end(),
						 std::bind2nd(std::ptr_fun(NetMauMau::Common::isRank),
									  NetMauMau::Common::ICard::SEVEN))) {

			for(int p = 0; p < 4; ++p) {

				std::partition(myCards.begin(), myCards.end(),
							   std::bind2nd(std::ptr_fun(NetMauMau::Common::isSuit),
											suitCount[p].suit));

				const NetMauMau::Player::IPlayer::CARDS::value_type f =
					NetMauMau::Common::findRank(NetMauMau::Common::ICard::SEVEN,
												myCards.begin(), myCards.end());

				if(f) {
					state.setPowerSuit(f->getSuit());
					return f->getSuit() != NetMauMau::Common::ICard::SUIT_ILLEGAL ? ACEREOUNDCOND :
						   POWERSUITCOND;
				}
			}

		} else {
			NetMauMau::Common::ICard::SUIT s = getMaxPlayedOffSuit(state);
			state.setPowerSuit(s);
			return s != NetMauMau::Common::ICard::SUIT_ILLEGAL ? ACEREOUNDCOND : POWERSUITCOND;
		}

	} else {
		state.setPowerSuit(m_suit);
		return m_suit != NetMauMau::Common::ICard::SUIT_ILLEGAL ? ACEREOUNDCOND : POWERSUITCOND;
	}

	return POWERSUITCOND;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
