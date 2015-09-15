#include <cstdlib>
#include <ctime>
#include <SFML/Network.hpp>
#include <iostream> // DEBUG
#include "GameServer.h"

int main(void)
{
	srand((unsigned int)time(NULL));
	GameServer game;

	if (game.initialize())
	{
		game.run();
		return 0;
	}

	return 1;
}