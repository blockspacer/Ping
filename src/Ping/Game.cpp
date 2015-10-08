#include <SFML/System.hpp>
#include <thread>
#include <chrono>
#include "Game.h"


Game::Game(void)
	: localTime(0)
	, state(GameState::Startup)
	, countdown(3.0f)
{
	// Initialize the players
	player[0].paddle = &stage.topPaddle;
	player[0].score = 0;
	player[1].paddle = &stage.bottomPaddle;
	player[1].score = 0;
}


Game::~Game(void)
{
}

void Game::run()
{
    if (!initialize()) {
        state = GameState::Shutdown;
    }

	// Fixed time-step
	sf::Clock clock;
	const float dt = 1.0f / 60.0f;
	float accumulator = 0.0f;

	while (GameState::Shutdown != state) {
		// Update the game then draw
		float frameTime = clock.restart().asSeconds();

        if (frameTime > 0.25f) {
            frameTime = 0.25f;
        }

		accumulator += frameTime;
		localTime += frameTime;

		while (accumulator >= dt) {
			update(dt);
			accumulator -= dt;
		}

		draw();

		// Prevent CPU from killing itself
		long timeLeft = static_cast<long>(1000*dt - clock.restart().asMilliseconds());
		if (timeLeft > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(timeLeft));
		}
	}

	shutdown();
}
