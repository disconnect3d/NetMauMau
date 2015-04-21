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

#include "clientcardfactory.h"

#include "cardtools.h"

using namespace NetMauMau::Client;

CardFactory::CardFactory(const std::string &cardDesc) : ICardFactory(), m_cardDesc(cardDesc) {}

CardFactory::~CardFactory() {}

NetMauMau::Common::ICard *CardFactory::create(NetMauMau::Common::ICard::SUIT,
		NetMauMau::Common::ICard::RANK) const {

	NetMauMau::Common::ICard::SUIT suit = NetMauMau::Common::ICard::HEARTS;
	NetMauMau::Common::ICard::RANK rank = NetMauMau::Common::ICard::ACE;

	if(NetMauMau::Common::parseCardDesc(m_cardDesc, &suit, &rank)) {
		return new Card(suit, rank, m_cardDesc);
	} else {
		return 0L;
	}
}

CardFactory::Card::Card(NetMauMau::Common::ICard::SUIT s, NetMauMau::Common::ICard::RANK v,
						const std::string &desc) : m_suit(s), m_rank(v), m_desc(desc),
	m_descAnsi(NetMauMau::Common::createCardDesc(m_suit, m_rank, true)) {}

CardFactory::Card::~Card() {}

std::size_t CardFactory::Card::getPoints() const {
	return NetMauMau::Common::getCardPoints(m_rank);
}

const std::string &CardFactory::Card::description(bool ansi) const {
	return !ansi ? m_desc : m_descAnsi;
}

NetMauMau::Common::ICard::SUIT CardFactory::Card::getSuit() const {
	return m_suit;
}

NetMauMau::Common::ICard::RANK CardFactory::Card::getRank() const {
	return m_rank;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
