#pragma once
#include <deque>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "../Ping/GraphicsView.h"
#include "../Ping/Player.h"
#include "../Ping/Ball.h"
#include "../Ping/Game.h"
#include "../Ping/Message.h"

class GameClient : public Game
{
public:
	sf::RenderWindow window;
	GraphicsView graphics;
	sf::TcpSocket client;

	GameClient();
	virtual ~GameClient();
	virtual bool initialize();
	virtual void run();
	virtual void shutdown();
	virtual void draw();
	virtual void update(float deltaTime);
	virtual void setServerAddress(sf::IpAddress addr);

protected:
	int playerIndex;
	sf::IpAddress serverAddr;
	float localTime;

	void keyPressed(sf::Event const &ev);
	void keyReleased(sf::Event const &ev);
	virtual void playerScored(int playerNumber);
	void sendMessage(Message const *m);

	void pollMessages();

	bool drawBounds;
	bool drawGraphics;
	bool drawServerState;

	GameSnapshot serverState;
	GameSnapshot prevServerState;

	float ssTime;
	float pssTime;
};

