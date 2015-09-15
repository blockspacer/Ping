#include "GameSnapshot.h"
#include "Game.h"

GameSnapshot::GameSnapshot(Game const *game)
	: timeUntilRelease(game->timeUntilRelease)
{
	for (int i = 0; i < 2; i++)
	{
		score[i] = game->player[i].getScore();
		playerPos[i] = game->player[i].paddle.position;
	}

	ballPos = game->ball.position;
} //--------------------------------------------------------------------------//


GameSnapshot::~GameSnapshot(void)
{
} //--------------------------------------------------------------------------//
