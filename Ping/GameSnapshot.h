#pragma once
#include "PingMath.h"

class Game; // Forward declaration

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
	Vector2 playerPos[2];	///< The positions of the players
	Vector2 ballPos;		///< The position of the ball

	float timeUntilRelease;	///< The time left until the ball is released
};

