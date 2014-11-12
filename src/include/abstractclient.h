/*
 * Copyright 2014 by Heiko Schäfer <heiko@rangun.de>
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

/**
 * @file
 * @brief The main class of a client
 * @author Heiko Schäfer <heiko@rangun.de>
 *
 * @mainpage %NetMauMau Client API
 *
 * To use the @b %NetMauMau @b Client @b API please read at first how to implement
 * NetMauMau::Client::AbstractClient. @n Useful functions you'll find in NetMauMau::Common.
 *
 * Link your client against @c -lnetmaumauclient and @c -lnetmaumaucommon
 *
 * You can grab the latest source code at https://github.com/velnias75/NetMauMau\n
 * A proof of concept Qt client can be found at https://github.com/velnias75/NetMauMau-Qt-Client
 */

#ifndef NETMAUMAU_ABSTRACTCLIENT_H
#define NETMAUMAU_ABSTRACTCLIENT_H

#include <vector>

#include "icard.h"
#include "clientconnection.h"

namespace NetMauMau {

/// @brief Classes and functions used by clients only
namespace Client {

/**
 * @brief Main interface to communicate with the server
 *
 * A %NetMauMau %client needs to implement this class to receive the events. Than you can use it
 * \-- by example \-- like that in your code:@code
 * MyClient client("MyName"); // implements NetMauMau::Client::AbstractClient
 *
 * try {
 * 	client.play();
 * } catch(const NetMauMau::Common::Exception::SocketException &e) {
 * 	// your error handling
 * }
 * @endcode
 *
 * The client translates the server commands into calls of pure virtual functions,
 * which the client has to implement and to react accordingly.
 *
 * A proof of concept Qt client can be found at https://github.com/velnias75/NetMauMau-Qt-Client
 *
 * @note All data is transferred as UTF-8 encoded byte strings
 */
class _EXPORT AbstractClient {
	DISALLOW_COPY_AND_ASSIGN(AbstractClient)
public:
	/// @copydoc Connection::CAPABILITIES
	typedef Connection::CAPABILITIES CAPABILITIES;

	/// @copydoc Connection::PLAYERLIST
	typedef Connection::PLAYERLIST PLAYERLIST;

	/**
	 * @brief A vector of @c Common::ICard pointers
	 */
	typedef std::vector<Common::ICard *> CARDS;

	/**
	* @brief Statistics entry about the other player's card count
	*/
	typedef struct {
		std::string playerName; ///< the player's name
		std::size_t cardCount; ///< the player's card count
	} STAT;

	/**
	 * @brief A vector with statistics about the other player's card count
	 * @see STAT
	 */
	typedef std::vector<STAT> STATS;

	virtual ~AbstractClient();

	/**
	 * @brief Attempt to start a game on the servers
	 *
	 * @param timeout the time to wait for a connection, if @c NULL there will be no timeout
	 *
	 * @throw Common::Exception::SocketException if the connection failed
	 * @throw Client::Exception::TimeoutException if the connection attempt timed out
	 */
	void play(timeval *timeout = NULL) throw(NetMauMau::Common::Exception::SocketException);

	/**
	 * @brief Disconnects the client from the server
	 */
	void disconnect();

	/**
	 * @name Server query methods
	 * @{
	 */

	/**
	 * @brief Returns the server capabilities
	 *
	 * @param timeout the time to wait for a connection, if @c NULL there will be no timeout
	 *
	 * @throw Common::Exception::SocketException if the connection failed
	 * @throw Client::Exception::TimeoutException if the connection attempt timed out
	 *
	 * @return CAPABILITIES the server capabilities
	 */
	CAPABILITIES capabilities(timeval *timeout = NULL)
	throw(NetMauMau::Common::Exception::SocketException);

	/**
	 * @brief Returns the list of currently registered player names
	 *
	 * @param timeout the time to wait for a connection, if @c NULL there will be no timeout
	 * @return NetMauMau::Client::AbstractClient::PLAYERLIST
	 * the list of currently registered player names
	 */
	PLAYERLIST playerList(timeval *timeout = NULL)
	throw(NetMauMau::Common::Exception::SocketException);

	/**
	 * @brief Returns the version of the client's implemented protocol version
	 *
	 * You can retrieve major and minor version as following: @code
	 * uint16_t major = static_cast<uint16_t>(AbstractClient::getClientProtocolVersion() >> 16);
	 * uint16_t minor = static_cast<uint16_t>(AbstractClient::getClientProtocolVersion());
	 * @endcode
	 *
	 * @return uint32_t the protocol version
	 */
	static uint32_t getClientProtocolVersion() _CONST;

	/**
	 * @brief Gets the default port of the server
	 *
	 * @return uint16_t the default port of the server
	 */
	static uint16_t getDefaultPort() _CONST;

	/**
	 * @brief Gets the player's name
	 *
	 * @return std::string the player's name
	 */
	std::string getPlayerName() const;

	/// @}

protected:
	/**
	 * @brief Creates an @c AbstractClient instance
	 *
	 * Sets up all information to connect to a server
	 *
	 * @see play
	 *
	 * @param player the player's name
	 * @param server the server to connect to
	 * @param port the server port to connect to
	 */
	AbstractClient(const std::string &player, const std::string &server, uint16_t port);

	/**
	 * @name Server requests
	 * @{
	 */

	/**
	 * @brief The server requests a card to play
	 *
	 * @note If the client send an <em>illegal card</em> the client will be requsted to
	 * choose a card again. Before the client will receive the amount of extra cards
	 * to take by played out SEVEN rank cards.
	 *
	 * @see Common::getIllegalCard
	 *
	 * @param cards the player's cards
	 * @return NetMauMau::Common::ICard* the card the player wants to play
	 * or @c NULL if the player cannot play a card and/or suspends the turn or
	 * the <em>illegal card</em>
	 */
	virtual Common::ICard *playCard(const CARDS &cards) const = 0;

	/**
	 * @brief Gets the current Jack suit
	 *
	 * @return NetMauMau::Common::ICard::SUIT the current Jack suit
	 */
	virtual Common::ICard::SUIT getJackSuitChoice() const = 0;

	//@}

	/**
	 * @name Sever events
	 * @{
	 */

	/**
	 * @brief The server send a general message
	 *
	 * @param msg the general message
	 */
	virtual void message(const std::string &msg) = 0;

	/**
	 * @brief The server send a error message
	 *
	 * @param msg the error message
	 */
	virtual void error(const std::string &msg) = 0;

	/**
	 * @brief A new turn has started
	 *
	 * @param turn number of the current turn
	 */
	virtual void turn(std::size_t turn) const = 0;

	/**
	 * @brief The server sent statistics about the other player's card count
	 *
	 * @param stats the statistics about the other player's card count
	 */
	virtual void stats(const STATS &stats) const = 0;

	/**
	 * @brief The server announced the game is over
	 */
	virtual void gameOver() const = 0;

	/**
	 * @brief A new player joined the game
	 *
	 * @param player the new player's name
	 */
	virtual void playerJoined(const std::string &player) const = 0;

	/**
	 * @brief A player got rejected to join the game
	 *
	 * @param player the rejected player's name
	 */
	virtual void playerRejected(const std::string &player) const = 0;

	/**
	 * @brief A player suspends this turn
	 *
	 * @param player the suspending player's name
	 */
	virtual void playerSuspends(const std::string &player) const = 0;

	/**
	 * @brief A player played a card
	 *
	 * @param player the player's name
	 * @param card the card the player played
	 */
	virtual void playedCard(const std::string &player, const Common::ICard *card) const = 0;

	/**
	 * @brief A player has won the game
	 *
	 * @param player the player's name
	 * @param turn the number of the turn the player has won
	 */
	virtual void playerWins(const std::string &player, std::size_t turn) const = 0;

	/**
	 * @brief A player has lost the game
	 *
	 * @param player the player's name
	 * @param turn the number of the turn the player has lost
	 */
	virtual void playerLost(const std::string &player, std::size_t turn) const = 0;

	/**
	 * @brief A player picks up a card
	 *
	 * @note The card is @c NULL if the player is a remote player
	 *
	 * @param player the player's name
	 * @param card the card the player picked up
	 */
	virtual void playerPicksCard(const std::string &player, const Common::ICard *card) const = 0;

	/**
	 * @brief A player picks up an amount of cards
	 *
	 * @param player the player's name
	 * @param count the count of picked up cards
	 */
	virtual void playerPicksCard(const std::string &player, std::size_t count) const = 0;

	/**
	 * @brief Name of the next player
	 *
	 * @param player the next player's name
	 */
	virtual void nextPlayer(const std::string &player) const = 0;

	/**
	 * @brief Notes if suspending and taking a card possible
	 *
	 * If there are no more cards on the talon, except the open card, suspending and
	 * taking card a is not possible
	 *
	 * @param enable @c true if it is possible to take a card, @c false otherwise
	 * @return void
	 */
	virtual void enableSuspend(bool enable) const = 0;

	/**
	 * @brief The card set distributed to the player, or if the
	 * player picked up cards off the talon
	 *
	 * @param cards the card set given to the player
	 */
	virtual void cardSet(const CARDS &cards) const = 0;

	/**
	 * @brief The initial card
	 *
	 * @param card the initial card
	 */
	virtual void initialCard(const Common::ICard *card) const = 0;

	/**
	 * @brief The current open card
	 *
	 * If there is a suit chosen by a Jack the jackSuit contains it and can get converted to
	 * @c NetMauMau::Common::ICard::SUIT by NetMauMau::Common::symbolToSuit, else it will be
	 * a empty string.
	 *
	 * @see NetMauMau::Common::symbolToSuit
	 *
	 * @param card the current open card
	 * @param jackSuit the current jack suit
	 */
	virtual void openCard(const Common::ICard *card, const std::string &jackSuit) const = 0;

	/**
	 * @brief The player's played card got rejected
	 *
	 * @param player the player's name
	 * @param card the rejected card
	 */
	virtual void cardRejected(const std::string &player, const Common::ICard *card) const = 0;

	/**
	 * @brief The player's played card got accepted
	 *
	 * @param card the accepted card
	 */
	virtual void cardAccepted(const Common::ICard *card) const = 0;

	/**
	 * @brief The server announces a Jack suit
	 *
	 * @param suit the current Jack suit
	 */
	virtual void jackSuit(Common::ICard::SUIT suit) = 0;

	// @}

private:
	CARDS getCards(const std::vector<Common::ICard *>::size_type cnt = 0) const;

private:
	Connection m_connection;
	std::string m_pName;
	std::vector<Common::ICard *> m_cards;
	Common::ICard *m_openCard;
	bool m_disconnectNow;
};

}

}

#endif /* NETMAUMAU_ABSTRACTCLIENT_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
