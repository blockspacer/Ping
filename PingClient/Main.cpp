#include <cstdlib>
#include <ctime>
#include <SFML/Network.hpp>
#include <iostream> // DEBUG
#include <string>
#include "GameClient.h"

int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));
	GameClient game;

	if (argc > 1)
	{
		game.setServerAddress(argv[1]);
	}
	else
	{
		std::string serverIp;
		std::cout << "Host IP Address: ";
		std::cin >> serverIp;
		game.setServerAddress(serverIp);
	}

	if (game.initialize())
	{
		game.run();
		return 0;
	}

	return 1;
}