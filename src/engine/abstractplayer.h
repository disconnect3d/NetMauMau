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

#ifndef NETMAUMAU_PLAYER_ABSTRACTPLAYER_H
#define NETMAUMAU_PLAYER_ABSTRACTPLAYER_H

#include "iplayer.h"

#include "smartptr.h"

namespace NetMauMau {

namespace Player {

class AbstractPlayer : public virtual IPlayer {
	DISALLOW_COPY_AND_ASSIGN(AbstractPlayer)
public:
	virtual ~AbstractPlayer();

	virtual std::string getName() const;
	virtual int getSerial() const = 0;
	virtual bool isAIPlayer() const = 0;
	virtual bool isAlive() const = 0;

	virtual void setRuleSet(const RuleSet::IRuleSet *ruleset);
	virtual void setCardCountObserver(const ICardCountObserver *cco);
	virtual void setEngineConfig(const EngineConfig *engineCfg);

	virtual void receiveCard(const Common::ICardPtr &card) = 0;
	virtual void receiveCardSet(const CARDS &cards);

	virtual Common::ICardPtr requestCard(const Common::ICardPtr &uncoveredCard,
										 const Common::ICard::SUIT *jackSuit,
										 std::size_t takeCount) const = 0;
	virtual Common::ICard::SUIT getJackChoice(const Common::ICardPtr &uncoveredCard,
			const Common::ICardPtr &playedCard) const = 0;

	virtual bool getAceRoundChoice() const = 0;

	virtual bool cardAccepted(const Common::ICard *playedCard);
	virtual void cardPlayed(Common::ICard *playedCard);
	virtual void informAIStat(const IPlayer *player, std::size_t count);
	virtual void setNeighbourCardCount(std::size_t playerCount, std::size_t leftCount,
									   std::size_t rightCount);
	virtual void setDirChangeEnabled(bool dirChangeEnabled);
	virtual void talonShuffled();
	virtual void setNineIsEight(bool b);

	virtual REASON getNoCardReason(const Common::ICardPtr &uncoveredCard,
								   const Common::ICard::SUIT *suit) const _PURE;

	virtual std::size_t getCardCount() const = 0;
	virtual std::size_t getPoints() const;

	virtual void reset() throw();

	virtual const CARDS &getPlayerCards() const _CONST;

protected:
	explicit AbstractPlayer(const std::string &name);

	virtual void shuffleCards();

	const RuleSet::IRuleSet *getRuleSet() const _PURE;
	const std::vector<std::string> &getPlayedOutCards() const _CONST;
	std::size_t getPlayerCount() const _PURE;
	std::size_t getLeftCount() const _PURE;
	std::size_t getRightCount() const _PURE;
	bool hasTakenCards() const _PURE;

	// cppcheck-suppress functionConst
	void setCardsTaken(bool b);

	bool hasPlayerFewCards() const _PURE;
	bool nineIsEight() const _PURE;
	bool isDirChgEnabled() const _PURE;

	const EngineConfig *getEngineConfig() const _PURE;

	// cppcheck-suppress functionConst
	CARDS getPossibleCards(const Common::ICardPtr &uncoveredCard,
						   const Common::ICard::SUIT *suit) const;

	void notifyCardCountChange() const throw();
	bool isAceRoundAllowed() const;

	void pushCard(const Common::ICardPtr &card);

private:
	const std::string m_name;
	CARDS m_cards;
	mutable bool m_cardsTaken;
	const RuleSet::IRuleSet *m_ruleset;
	std::vector<std::string> m_playedOutCards;
	bool m_playerHasFewCards;
	bool m_nineIsEight;
	std::size_t m_leftCount;
	std::size_t m_rightCount;
	bool m_dirChgEnabled;
	std::size_t m_playerCount;
	const EngineConfig *m_engineCfg;
	const ICardCountObserver *m_cardCountObserver;
};

}

}

#endif /* NETMAUMAU_PLAYER_ABSTRACTPLAYER_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
