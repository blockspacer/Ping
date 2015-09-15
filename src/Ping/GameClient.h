#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameSnapshot.h"

/// The GameClient establishes a connection to a GameServer. In additional to
/// running a similar simulation, the GameClient renders the Stage to the
/// screen and accepts user input to control the Player's paddle. The
/// GameClient sends input events to the server and receives the state of the
/// server at regular intervals, in order to stay in sync with it.
class GameClient : public Game
{
public:
	sf::RenderWindow window;	///< The window that will render the Stage
	sf::IpAddress serverAddr;	///< The address of the server to connect to
	sf::TcpSocket server;

	GameClient(void);
	~GameClient(void);

	// Interface
	virtual bool initialize();
	virtual void shutdown();
	virtual void update(float deltaTime);
	virtual void draw();
	void setServerAddr(const char* addr) { serverAddr = sf::IpAddress(addr); }

protected:
	int playerIndex;	///< Are we controlling player 0 or 1?
	bool predict;		///< Should we use client-side prediction?
	bool interpolate;	///< Should we use client-side interpolation?

	GameSnapshot serverState;		///< The most recent server snapshot
	GameSnapshot prevServerState;	///< The previous server snapshot
	float ssTime;					///< Local time of most recent snapshot
	float psTime;					///< Local time of previous snapshot

	/// Handles the user keyboard input.
	void handleInput();

	/// Check to see whether server has sent us any data
	void pollMessages();

	/// Initializes and opens the display window
	void openDisplay();

	/// Draw the score to the display window
	void drawScore();

	/// Draw countdown to the display window
	void drawCountdown();

private:
	// Shapes for rendering the Stage to the screen
	sf::RectangleShape topPaddle;
	sf::RectangleShape bottomPaddle;
	sf::CircleShape ball;
	sf::Font font;
};

