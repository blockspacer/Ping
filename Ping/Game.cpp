#include "Game.h"

void Game::updateHistory()
{
	history.push_back(GameSnapshot(this));
	if (history.size() > 3)
	{
		history.pop_front();
	}
} //--------------------------------------------------------------------------//
