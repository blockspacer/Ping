#pragma once
#include "IUpdateable.h"
#include "Stage.h"

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

namespace Message
{
	/// Values that represent the different Packet Types.
	enum
	{
		//---------------------------------------------
		// Type				      Additional data sent 
		//---------------------------------------------
		ClientConnected,	///< (int)playerIndex
		GameStarted,
		PlayerMoveLeft,		///< (bool)value
		PlayerMoveRight,	///< (bool)value
		PlayerScored,		///< (int)playerIndex
		BallReleased,
		GameSync
	};
}

/// A simple Player structure to hold the score and the paddle he/she controls.
struct Player
{
	int score;
	Paddle *paddle;
};

/// An Abstract Game class. The heart of the program is the Game.
/// It controls the simulaton and contains all of the core functionality.
class Game : public IUpdateable
{
public:
	int state;			///< The current state of the Game
	float localTime;	///< Local seconds running
	float countdown;	///< Countdown until ball is released
	Stage stage;		///< The stage containing the paddles and ball
	Player player[2];	///< The two players playing the game

	Game(void);
	void run();
	~Game(void);

	//====================
	// Interface
	//====================
	
	/// Called after construction, immediately before entering the update loop.
	/// Connection establishment and resource loading should go here. If this
	/// method returns false, the Game will not enter the update loop, and it
	/// will immediately switch to the Shutdown state.
	virtual bool initialize() = 0;

	/// Called after the update loop has finished. Frees any resources used by
	/// the game before exiting.
	virtual void shutdown() = 0;

	/// Called on every tick. Because we're running at a fixed time-step, this
	/// should be called at a regular interval.
	/// @param	deltaTime	The time since the last update (seconds).
	virtual void update(float deltaTime) = 0;

	/// Draw the Game
	virtual void draw() { }
};

