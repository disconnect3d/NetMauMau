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

#ifndef NETMAUMAU_TALON_H
#define NETMAUMAU_TALON_H

#include <cstddef>                      // for size_t
#include <stack>                        // for stack

#include "iplayedoutcards.h"            // for IPlayedOutCards::CARDS, etc

namespace NetMauMau {

class ITalonChange;

class Talon : public virtual IPlayedOutCards {
	DISALLOW_COPY_AND_ASSIGN(Talon)
public:
	typedef std::stack<CARDS::value_type, CARDS> CARDSTACK;

	explicit Talon(ITalonChange *tchg, std::size_t factor) throw();
	virtual ~Talon() throw();

	inline bool thresholdReached(std::size_t t = 1) const throw() {
		return empty() && m_uncovered.size() <= t;
	}

	inline bool empty() const throw() {
		return m_cardStack.empty();
	}

	inline Common::ICardPtr top() const throw() {
		return m_cardStack.top();
	}

	inline void pop() throw() {
		m_cardStack.pop();
	}

	virtual const CARDS &getCards() const throw();

	Common::ICardPtr uncoverCard() throw();

	inline Common::ICardPtr getUncoveredCard() const throw() {
		return m_uncovered.top();
	}

	void playCard(const Common::ICardPtr &card) throw();

	Common::ICardPtr takeCard() throw();

	inline void pushBackCard(const Common::ICardPtr &c) throw() {
		m_cardStack.push(c);
	}

	void reset() throw();

private:
	static CARDSTACK::container_type createCards(std::size_t factor) throw();

	void emitUnderFlow() const throw();

private:
	ITalonChange *m_talonChangeListener;
	mutable CARDS m_playedOutCards;
	CARDSTACK m_cardStack;
	CARDSTACK m_uncovered;
	mutable bool m_uncoveredDirty;
	const std::size_t m_factor;
};

}

#endif /* NETMAUMAU_TALON_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
