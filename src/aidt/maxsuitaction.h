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

#ifndef NETMAUMAU_ENGINE_AIDT_MAXSUITACTION_H
#define NETMAUMAU_ENGINE_AIDT_MAXSUITACTION_H

#include "abstractaction.h"

namespace NetMauMau {

namespace AIDT {

class MaxSuitAction : public AbstractAction {
	DISALLOW_COPY_AND_ASSIGN(MaxSuitAction)
public:
	MaxSuitAction();
	virtual ~MaxSuitAction();

	virtual const IConditionPtr &operator()(IAIState &state) const;

private:
	static Common::ICardPtr hasRankPath(const Common::ICardPtr &uc, Common::ICard::SUIT s,
										Common::ICard::RANK r, const Player::IPlayer::CARDS &cards,
										bool nineIsEight);
};

}

}

#endif /* NETMAUMAU_ENGINE_AIDT_MAXSUITACTION_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
