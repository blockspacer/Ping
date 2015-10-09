#include "Ping/GameClient.h"
#include "Ping/GameServer.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>

int main()
{
    std::string input;
    bool validOption = false;
    std::unique_ptr<Game> game;

    while (!validOption) {
        std::cout
            << "------------------------------" << std::endl
            << "             PING             " << std::endl
            << "------------------------------" << std::endl
            << "[s]erver  [c]lient  [q]uit : ";
        std::cin >> input;

        if (input[0] == 's' || input[0] == 'S') {
            validOption = true;
            game = std::make_unique<GameServer>();
        }
        else if (input[0] == 'c' || input[0] == 'C') {
            validOption = true;
            game = std::make_unique<GameClient>();
            std::cout << "Connect to address: ";
            std::cin >> input;
            dynamic_cast<GameClient*>(game.get())->setServerAddr(input.c_str());
        }
        else if (input[0] == 'q' || input[0] == 'Q') {
            return EXIT_SUCCESS;
        }
    }
    
    game->run();

    return EXIT_SUCCESS;
}
