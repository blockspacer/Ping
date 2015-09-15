#include "GameServer.h"
#include <iostream>

GameServer::GameServer()
	: numClients(0)
	, syncRate(1.0f/20.0f)
	, timeUntilSync(syncRate)
{
} // ---------------------------------------------------------------------- //


bool GameServer::initialize()
{
	std::cout
		<< "-------------" << std::endl
		<< " PING SERVER " << std::endl
		<< "-------------" << std::endl
		<< "Listening on " << sf::IpAddress::getLocalAddress() << ":44000" << std::endl;

	// Create a listener to wait for incoming connections on port 44000
	if (serverSocket.listen(44000) != sf::Socket::Done)
		return false;

	// Add the server socket to the selector
	selector.add(serverSocket);

	player[0].initialize(1);
	player[1].initialize(2);
	client[0].setBlocking(false);
	client[1].setBlocking(false);

	timeUntilRelease = 3.0f;

	changeState(GameState::WaitingForClients);
	return true;
} // ------------------------------------------------------------------------ //


void GameServer::update(float deltaTime)
{
	if (GameState::WaitingForClients == state)
	{
		checkForClients();
		return;
	}

	player[0].update(deltaTime);
	player[1].update(deltaTime);
	ball.update(deltaTime);

	if (state == GameState::Preparatory)
	{
		if (timeUntilRelease <= 0)
		{
			changeState(GameState::Playing);
			releaseBall();
		}
		else if (timeUntilRelease <= 2)
		{
			ball.reset();
			timeUntilRelease -= deltaTime;
		}
		else
			timeUntilRelease -= deltaTime;
	}

	if (state == GameState::Playing)
	{
		checkBallForGoal();
	}

	updateHistory();

	if (timeUntilSync > 0)
	{
		timeUntilSync -= deltaTime;
	}
	else
	{
		GameSyncMessage m;
		m.type = MessageType::GameSync;
		m.snapshot = history.back();
		sendMessage(&m);
		timeUntilSync = syncRate;
	}
} // ------------------------------------------------------------------------ //


void GameServer::releaseBall()
{
	float angle = ball.generateAngle();
	BallReleasedMessage m;
	m.type = MessageType::BallReleased;
	m.angle = angle;
	sendMessage(&m);
	ball.release(ball.generateAngle());
}

void GameServer::checkBallForGoal()
{
	float p0X = player[0].paddle.position.x;
	float p1X = player[1].paddle.position.x;

	float offset = 50.0f;

	// Check to see if paddle incercepted top/bottom collision
	if (ball.position.y + Ball::Radius > Height - offset)
	{
		if (PingMath::between(ball.position.x, p1X - Paddle::Width / 2.0f - Ball::Radius, p1X + Paddle::Width / 2.0f + Ball::Radius))
		{
			ball.position.y = Game::Height - Ball::Radius - 1.0f - offset;
			ball.direction.y = -ball.direction.y;
			ball.direction.x = PingMath::clamp(ball.direction.x + player[0].paddle.getSpeed() * 0.0005f, -0.9f, 0.9f);
			PingMath::normalize(ball.direction);
		}
		else
		{
			playerScored(1);
		}
	}
	else if (ball.position.y - Ball::Radius < offset)
	{
		if (PingMath::between(ball.position.x, p0X - Paddle::Width / 2.0f - Ball::Radius, p0X + Paddle::Width / 2.0f + Ball::Radius))
		{
			ball.position.y = Ball::Radius + 1.0f + offset;
			ball.direction.y = -ball.direction.y;
			ball.direction.x = PingMath::clamp(ball.direction.x + player[1].paddle.getSpeed() * 0.0005f, -0.9f, 0.9f);
			PingMath::normalize(ball.direction);
		}
		else
		{
			playerScored(0);
		}
	}
}

void GameServer::playerScored(int index)
{
	player[index].increaseScore();

	timeUntilRelease = 3.0f;
	changeState(GameState::Preparatory);

	PlayerScoredMessage m;
	m.type = MessageType::PlayerScored;
	m.playerIndex = index;
	sendMessage(&m);

	std::cout << "Score: " << player[0].getScore() << " | " << player[1].getScore() << std::endl;
}


void GameServer::run()
{
	sf::Clock clock;

	const float dt = 1.0f / 60.0f;
	float accumulator = 0.0f;

	while (GameState::Shutdown != state)
	{
		pollMessages();

		// Update the GameServer
		float frameTime = clock.restart().asSeconds();

		if (frameTime > 0.25f)
			frameTime = 0.25f;

		accumulator += frameTime;
		while (accumulator >= dt)
		{
			update(dt);
			accumulator -= dt;
		}
	}
	shutdown();
} // ------------------------------------------------------------------------ //

void GameServer::checkForClients()
{
	// Check whether any sockets are flagged as ready
	if (selector.wait())
	{
		// Check if server socket is ready (there is a pending connection)
		if (selector.isReady(serverSocket))
		{
			if (serverSocket.accept(client[numClients]) == sf::Socket::Done)
			{
				// Add the new client to the selector so we are notified
				// when it wishes to send data
				selector.add(client[numClients]);
				sf::IpAddress remoteIp = client[numClients].getRemoteAddress();
				std::cout << "Client " << numClients << " connected from " << remoteIp << std::endl;

				ClientConnectedMessage m;
				m.type = MessageType::ClientConnected;
				m.playerIndex = numClients;
				numClients++;
				sendMessage(&m);
			}
			if (numClients == 2)
			{
				Message m;
				m.type = MessageType::GameStarted;
				sendMessage(&m);
				changeState(GameState::Preparatory);
				std::cout << "Starting Game" << std::endl;
			}
		}
	}
}

void GameServer::sendMessage(Message const *m)
{
	sf::Packet packet;
	switch (m->type)
	{
	case MessageType::GameStarted :
	case MessageType::GameOver :
		packet << *m;
	case MessageType::PlayerScored :
		packet << *(PlayerScoredMessage*)m;
		break;
	case MessageType::GameSync :
		packet << *(GameSyncMessage*)m;
		break;
	case MessageType::BallReleased :
		packet << *(BallReleasedMessage*)m;
		break;
	case MessageType::ClientConnected :
		packet << *(ClientConnectedMessage*)m;
		break;
	}

	for (int i = 0; i < numClients; i++)
	{
		client[i].send(packet);
	}
}

void GameServer::pollMessages()
{
	// Check for clients sending us data
	for (int i = 0; i < numClients; i++)
	{
		if (true)
		{
			// The client has sent some data, we can receive it
			int type;
			sf::Packet packet;
			int status = client[i].receive(packet);
			if (sf::Socket::Done == status)
			{
				packet >> type;
				if (MessageType::PlayerMove == type)
				{
					PlayerMoveMessage m;
					packet >> m;
					if (m.right)
						player[i].paddle.setMoveRight(m.value);
					else
						player[i].paddle.setMoveLeft(m.value);
				}
			}
			else if (sf::Socket::Disconnected == status)
			{
				// Game over
				selector.remove(client[i]);
				client[i].disconnect();
				changeState(GameState::Shutdown);
				std::cout << "Client disconnected, shutting down" << std::endl;
			}
		}
	}
}

void GameServer::shutdown()
{
} // ------------------------------------------------------------------------ //


GameServer::~GameServer()
{
} // ---------------------------------------------------------------------- //
