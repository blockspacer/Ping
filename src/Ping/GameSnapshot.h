#pragma once
#include <SFML/Network.hpp>
#include "PingMath.h"

// Forward declaration
class Game;

/// A GameSnapshot is a lightweight representation of a Game at a particular
/// point in time.
class GameSnapshot
{
public:

	/// Constructor.
	/// @param	game	The game to take a snapshot of.
	GameSnapshot(Game const *game);
	GameSnapshot(void) { }
	virtual ~GameSnapshot(void);

	int score[2];			///< The scores of the players
	Vector2 paddlePos[2];	///< The positions of the top and bottom paddles
	Vector2 ballPos;		///< The position of the ball
};

/// Packet building instructions
inline sf::Packet& operator <<(sf::Packet& Packet, const GameSnapshot& g)
{
	for (int i = 0; i < 2; i++)
	{
		Packet
			<< g.paddlePos[i].x
			<< g.paddlePos[i].y
			<< g.score[i];
	}
	return Packet << g.ballPos.x << g.ballPos.y;
}

/// Packet reading instructions
inline sf::Packet& operator >>(sf::Packet& Packet, GameSnapshot& g)
{
	for (int i = 0; i < 2; i++)
	{
		Packet
			>> g.paddlePos[i].x
			>> g.paddlePos[i].y
			>> g.score[i];
	}
	return Packet >> g.ballPos.x >> g.ballPos.y;
}