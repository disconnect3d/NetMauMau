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

#ifndef NETMAUMAU_ENGINE_AIDT_ABSTRACTACTION_H
#define NETMAUMAU_ENGINE_AIDT_ABSTRACTACTION_H

#include "icard.h"
#include "iplayer.h"
#include "iaction.h"
#include "icondition.h"

namespace NetMauMau {

namespace Engine {

namespace AIDT {

class AbstractAction : public IAction {
	DISALLOW_COPY_AND_ASSIGN(AbstractAction)
public:
	virtual ~AbstractAction();

protected:
	AbstractAction();

	static Player::IPlayer::CARDS pullSuit(const AIState &state,
										   NetMauMau::Common::ICard::SUIT suit);

	static const IConditionPtr &getNullCondition() _CONST;
};

}

}

}

#endif /* NETMAUMAU_ENGINE_AIDT_ABSTRACTACTION_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
