#pragma once
#include <deque>
#include "IUpdateable.h"
#include "GameSnapshot.h"
#include "Player.h"
#include "Ball.h"

namespace GameState
{
	/// Values that represent the possible GameStates.
	enum
	{
		Startup,
		WaitingForClients,
		Preparatory,
		Playing,
		Shutdown
	};
}

/// An Abstract Game class. The heart of the program is the Game.
/// It controls the simulaton an contains all of the core functionality.
class Game : public IUpdateable
{
public:

	// Public Members
	static const int Width = 1280;
	static const int Height = 720;
	float timeUntilRelease;
	Player player[2];
	Ball ball;

	// Public Methods
	Game() : state(GameState::Startup) { }
	virtual ~Game() { }
	void changeState(int newState) { state = newState; }

	// Interface
	virtual bool initialize() = 0;
	virtual void run() = 0;
	virtual void shutdown() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void playerScored(int playerIndex) = 0;

protected:
	int state;	///< The current state of the Game
	
	std::deque<GameSnapshot> history;
	void updateHistory();
};

