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

#ifndef NETMAUMAU_ENGINE_AIDT_IACTION_H
#define NETMAUMAU_ENGINE_AIDT_IACTION_H

#include "linkercontrol.h"

namespace NetMauMau {

namespace Common {
template<class> class SmartPtr;
}

namespace Engine {

namespace AIDT {

class ICondition;
class AIState;

class IAction {
	DISALLOW_COPY_AND_ASSIGN(IAction)
public:
	virtual ~IAction() {}

	virtual const Common::SmartPtr<ICondition> &operator()(AIState &state) const = 0;

protected:
	IAction() {}
};

typedef Common::SmartPtr<IAction> IActionPtr;

}

}

}

#endif /* NETMAUMAU_ENGINE_AIDT_IACTION_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
