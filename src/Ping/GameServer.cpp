#include <iostream>
#include "GameServer.h"
#include "GameSnapshot.h"

GameServer::GameServer(void)
	: syncRate(1/20.0f)
	, timeUntilSync(syncRate)
{
	client[0].player = &player[0];
	client[1].player = &player[1];
}


bool GameServer::initialize()
{
	std::cout << "Listening on " <<
		sf::IpAddress::getLocalAddress() << ":44000" << std::endl;

	// Create a listener to wait for incoming connections on port 44000
    if (serverSocket.listen(44000) != sf::Socket::Done) {
        return false;
    }

	// Wait for client connections
    if (!waitForClients()) {
        return false;
    }

	// Don't block when checking for data from now on
	client[0].socket.setBlocking(false);
	client[1].socket.setBlocking(false);

	// Inform the clients the game is starting
	sf::Packet p;
	p << Message::GameStarted; // Always send message type first
	sendMessage(p);

	std::cout << "Starting Game" << std::endl;
	state = GameState::Preparatory;
	countdown = 3.0f;

	// Receive ball hit top/bottom notifications from Stage
	stage.setObserver(this);
	return true;
}


void GameServer::update(float deltaTime)
{
	pollMessages();

	if (state == GameState::Playing) {
		stage.update(deltaTime);
	}
	else if (state == GameState::Preparatory) {
		if (countdown <= 0) {
			// Release the ball and notify clients
			sf::Packet p;
			p << Message::BallReleased << stage.ball.angle;
			sendMessage(p);
			state = GameState::Playing;
			stage.start();
		}
		else {
			countdown -= deltaTime;
		}
	}

	timeUntilSync -= deltaTime;
	if (timeUntilSync <= 0) {
		// Send the clients a snapshot of the game
		sf::Packet p;
		p << Message::GameSync << GameSnapshot(this);
		sendMessage(p);
		timeUntilSync = syncRate;
	}
}


void GameServer::ballHitTop()
{
	// Increase score and notify players
	player[1].score++;
	sf::Packet p;
	p << Message::PlayerScored << 1;
	sendMessage(p);

	// Reset the stage
	stage.reset();
	state = GameState::Preparatory;
	countdown = 3.0f;

}


void GameServer::ballHitBottom()
{
	// Increase score and notify players
	player[0].score++;
	sf::Packet p;
	p << Message::PlayerScored << 0;
	sendMessage(p);

	// Reset the stage
	stage.reset();
	state = GameState::Preparatory;
	countdown = 3.0f;
}


bool GameServer::waitForClients()
{
	state = GameState::WaitingForClients;

	// We want 2 clients
	for (int i = 0; i < 2; i++) {
		if (serverSocket.accept(client[i].socket) == sf::Socket::Done) {
			sf::IpAddress remoteIp = client[i].socket.getRemoteAddress();
			std::cout << "Client " << i << " connected from " << remoteIp << std::endl;

			// Inform the client that they have connected
			sf::Packet p;
			p << Message::ClientConnected;	// Always send message type first
			p << i;							// Inform the client of their index
			client[i].socket.send(p);
		}
		else {
			std::cout << "Connection error." << std::endl;
			return false;
		}
	}
	return true;
}


void GameServer::sendMessage(sf::Packet &p)
{
	client[0].socket.send(p);
	client[1].socket.send(p);
}


void GameServer::pollMessages()
{
	for (int i = 0; i < 2; i++) {
		// The client has sent some data, we can receive it
		int type;
		sf::Packet packet;
		int status = client[i].socket.receive(packet);
		if (sf::Socket::Done == status) {
			packet >> type;
            if (type == Message::PlayerMoveLeft) {
                packet >> client[i].player->paddle->moveLeft;
            }
            if (type == Message::PlayerMoveRight) {
                packet >> client[i].player->paddle->moveRight;
            }
		}
		else if (sf::Socket::Disconnected == status) {
			// Exit
			client[i].socket.disconnect();
			state = GameState::Shutdown;
			std::cout << "Client disconnected, shutting down." << std::endl;
		}
	}
}


void GameServer::shutdown()
{
}

GameServer::~GameServer(void)
{
}
