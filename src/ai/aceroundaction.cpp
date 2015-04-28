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

#include "aceroundaction.h"

#include "randomjackcondition.h"
#include "iaistate.h"
#include "iruleset.h"

namespace {
NetMauMau::AI::IConditionPtr RANDOMJACKCOND(new NetMauMau::AI::RandomJackCondition());
}

using namespace NetMauMau::AI;

AceRoundAction::AceRoundAction() : AbstractAction() {}

AceRoundAction::~AceRoundAction() {}

const IConditionPtr &AceRoundAction::perform(IAIState &state,
		const NetMauMau::Player::IPlayer::CARDS &) const {

	NetMauMau::Player::IPlayer::CARDS myCards(state.getPlayerCards());

	state.setTryAceRound(DecisionBase::countRank(myCards, state.getRuleSet()->getAceRoundRank())
						 > (state.tryAceRound() ? 0 : 1));

	if(state.tryAceRound()) {

		AbstractAction::pullRank(myCards, state.getRuleSet()->getAceRoundRank());
		state.setCard(*myCards.begin());

		return AbstractAction::getNullCondition();
	}

	return RANDOMJACKCOND;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
