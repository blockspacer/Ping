#pragma once
#include <deque>
#include <SFML/Network.hpp>
#include "../Ping/Game.h"
#include "../Ping/Message.h"


class GameServer : public Game
{
public:
	int numClients;
	sf::SocketSelector selector;	// Use a selector to support multiple clients
	sf::TcpListener serverSocket;
	sf::TcpSocket client[2];

	GameServer();
	~GameServer();
	virtual bool initialize();
	virtual void run();
	virtual void shutdown();
	virtual void update(float deltaTime);

private:
	void checkBallForGoal();
	void releaseBall();
	virtual void playerScored(int playerNumber);
	void checkForClients();
	void pollMessages();

	const float syncRate;
	float timeUntilSync;

	void sendMessage(Message const *m);
};

