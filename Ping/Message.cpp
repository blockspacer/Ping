#include <SFML/Network.hpp>
#include "Message.h"

// Packet building instructions
sf::Packet& operator <<(sf::Packet& Packet, const Message& m)
{
    return Packet << m.type;
}
sf::Packet& operator >>(sf::Packet& Packet, Message& m)
{
    return Packet >> m.type;
}

// Packet building instructions
sf::Packet& operator <<(sf::Packet& Packet, const PlayerMoveMessage& m)
{
    return Packet << m.type << m.right << m.value;
}
sf::Packet& operator >>(sf::Packet& Packet, PlayerMoveMessage& m)
{
    return Packet >> m.right >> m.value;
}


// Packet building instructions
sf::Packet& operator <<(sf::Packet& Packet, const PlayerScoredMessage& m)
{
    return Packet << m.type << m.playerIndex;
}
sf::Packet& operator >>(sf::Packet& Packet, PlayerScoredMessage& m)
{
    return Packet >> m.playerIndex;
}


// Packet building instructions
sf::Packet& operator <<(sf::Packet& Packet, const GameSyncMessage& m)
{
	Packet << m.type;
	for (int i = 0; i < 2; i++)
	{
		Packet
			<< m.snapshot.playerPos[i].x
			<< m.snapshot.playerPos[i].y
			<< m.snapshot.score[i];
	}
	return Packet << m.snapshot.ballPos.x << m.snapshot.ballPos.y;
}
sf::Packet& operator >>(sf::Packet& Packet, GameSyncMessage& m)
{
	for (int i = 0; i < 2; i++)
	{
		Packet
			>> m.snapshot.playerPos[i].x
			>> m.snapshot.playerPos[i].y
			>> m.snapshot.score[i];
	}
	return Packet >> m.snapshot.ballPos.x >> m.snapshot.ballPos.y;
}

// Packet building instructions
sf::Packet& operator <<(sf::Packet& Packet, const BallReleasedMessage& m)
{
    return Packet << m.type << m.angle;
}
sf::Packet& operator >>(sf::Packet& Packet, BallReleasedMessage& m)
{
    return Packet >> m.angle;
}

// Packet building instructions
sf::Packet& operator <<(sf::Packet& Packet, const ClientConnectedMessage& m)
{
    return Packet << m.type << m.playerIndex;
}
sf::Packet& operator >>(sf::Packet& Packet, ClientConnectedMessage& m)
{
    return Packet >> m.playerIndex;
}