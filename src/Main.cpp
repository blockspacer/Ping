#include "Ping/GameClient.h"
#include "Ping/GameServer.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

	std::string input;
	bool validOption = false;
	Game *game;

	while (!validOption)
	{
		std::cout
			<< "------------------------------" << std::endl
			<< "             PING             " << std::endl
			<< "------------------------------" << std::endl
			<< "[s]erver  [c]lient  [q]uit : ";
		std::cin >> input;

		if (input[0] == 's' || input[0] == 'S')
		{
			validOption = true;
			game = new GameServer;
		}
		else if (input[0] == 'c' || input[0] == 'C')
		{
			validOption = true;
			game = new GameClient;
			std::cout << "Connect to address: ";
			std::cin >> input;
			((GameClient*)game)->setServerAddr(input.c_str());
		}
		else if (input[0] == 'q' || input[0] == 'Q')
		{
			return EXIT_SUCCESS;
		}
	}
	
	game->run();

	delete game;
    return EXIT_SUCCESS;
}
