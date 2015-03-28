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

#ifndef NETMAUMAU_STDCARDFACTORY_H
#define NETMAUMAU_STDCARDFACTORY_H

#include "icardfactory.h"

namespace NetMauMau {

class StdCardFactory : public ICardFactory {
public:
	explicit StdCardFactory();
	~StdCardFactory();

	virtual _NOUNUSED Common::ICard *create(Common::ICard::SUIT s, Common::ICard::RANK r) const;

private:
	class StdCard : public Common::ICard {
		DISALLOW_COPY_AND_ASSIGN(StdCard)
	public:
		explicit StdCard(ICard::SUIT f, ICard::RANK r);
		virtual ~StdCard();

		virtual SUIT getSuit() const _PURE;
		virtual RANK getRank() const _PURE;
		virtual std::size_t getPoints() const _PURE;

		virtual std::string description(bool ansi) const;

	private:
		const ICard::SUIT m_suit;
		const ICard::RANK m_rank;
	};
};

}

#endif /* NETMAUMAU_STDCARDFACTORY_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
