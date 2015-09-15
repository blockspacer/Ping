#pragma once
#include "GameSnapshot.h"


/// A Message is the basic structure of communication. Messages are passed
/// between the client and server and carry important information.
struct Message
{
	int type;
};

namespace MessageType
{
	/// Values that represent the different Message Types.
	enum
	{
		ClientConnected,
		GameStarted,
		GameOver,
		PlayerMove,
		PlayerScored,
		BallReleased,
		GameSync
	};
};

/// The Message sent to the server when Player input is received.
struct PlayerMoveMessage : public Message
{
	bool right; ///< Is the Player moving to the right? If not, assume left
	bool value;	///< True if we are starting to move, false if stopping.
};

/// The Message sent to the clients from the server when a Player scores.
struct PlayerScoredMessage : public Message
{
	int playerIndex;	///< The Player that scored
};

/// The Message sent to the clients immediately after they join the server.
struct ClientConnectedMessage : public Message
{
	int playerIndex;	///< The Player number that was assigned to the client
};

/// The Message sent to clients informing them of the current server's snapshot
struct GameSyncMessage : public Message
{
	GameSnapshot snapshot;
};

/// The Message sent to clients when the Ball is put in motion.
struct BallReleasedMessage : public Message
{
	float angle;	///< The angle the Ball should move at
};

// SFML Packets provide a convenient way to send data over a network.
// We create these functions to provide an easy way to build packets out of
// Messages and vice versa.
sf::Packet& operator <<(sf::Packet& Packet, const Message& m);
sf::Packet& operator >>(sf::Packet& Packet, Message& m);
sf::Packet& operator <<(sf::Packet& Packet, const PlayerMoveMessage& m);
sf::Packet& operator >>(sf::Packet& Packet, PlayerMoveMessage& m);
sf::Packet& operator <<(sf::Packet& Packet, const PlayerScoredMessage& m);
sf::Packet& operator >>(sf::Packet& Packet, PlayerScoredMessage& m);
sf::Packet& operator <<(sf::Packet& Packet, const GameSyncMessage& m);
sf::Packet& operator >>(sf::Packet& Packet, GameSyncMessage& m);
sf::Packet& operator <<(sf::Packet& Packet, const BallReleasedMessage& m);
sf::Packet& operator >>(sf::Packet& Packet, BallReleasedMessage& m);

sf::Packet& operator <<(sf::Packet& Packet, const ClientConnectedMessage& m);
sf::Packet& operator >>(sf::Packet& Packet, ClientConnectedMessage& m);