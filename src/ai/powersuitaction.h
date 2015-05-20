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

#ifndef NETMAUMAU_ENGINE_AI_POWERSUITACTION_H
#define NETMAUMAU_ENGINE_AI_POWERSUITACTION_H

#include "abstractaction.h"             // for AbstractAction

namespace NetMauMau {

namespace AI {

class PowerSuitAction : public AbstractAction {
	DISALLOW_COPY_AND_ASSIGN(PowerSuitAction)
public:
	PowerSuitAction();
	explicit PowerSuitAction(Common::ICard::SUIT suit);
	virtual ~PowerSuitAction();

	virtual const IConditionPtr &perform(IAIState &state,
										 const Player::IPlayer::CARDS &cards) const;
#if defined(TRACE_AI) && !defined(NDEBUG)
protected:
	inline virtual std::string traceLog() const {
		return "PowerSuitAction";
	}
#endif

private:
	const bool m_determineSuit;
	Common::ICard::SUIT m_suit;
};

}

}

#endif /* NETMAUMAU_ENGINE_AI_POWERSUITACTION_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
