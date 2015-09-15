#include <iostream>
#include "GameClient.h"

GameClient::GameClient()
	: drawBounds(false)
	, drawServerState(false)
	, drawGraphics(true)
	, playerIndex(-1)
	, localTime(0.0f)
{
} // ---------------------------------------------------------------------- //


bool GameClient::initialize()
{
	std::cout
		<< "-------------" << std::endl
		<< " PING CLIENT " << std::endl
		<< "-------------" << std::endl
		<< "Connecting to " << serverAddr.toString() << "..." << std::endl;

	bool ok = false;
	ok = graphics.loadContent();

	if (serverAddr == sf::IpAddress::None)
	{
		return false;
	}
	if (client.connect(serverAddr, 44000) != sf::Socket::Done)
	{
		return false;
	}
	client.setBlocking(false);

	timeUntilRelease = 3.0f;
	return ok;
} // ------------------------------------------------------------------------ //

void GameClient::setServerAddress(sf::IpAddress addr)
{
	serverAddr = addr;
}

void GameClient::update(float deltaTime)
{
	if (GameState::WaitingForClients == state || GameState::Startup == state)
	{
		return;
	}

	player[playerIndex].update(deltaTime);
	//ball.update(deltaTime);

	if (state == GameState::Playing)
	{
		//checkBallForGoal();
		int temp = 0;
	}

	if (timeUntilRelease > 0)
		timeUntilRelease -= deltaTime;

	updateHistory();
} // ------------------------------------------------------------------------ //

void GameClient::playerScored(int index)
{
	player[index].increaseScore();
	ball.reset();

	timeUntilRelease = 3.0f;
	changeState(GameState::Preparatory);

	std::cout << "Score: " << player[0].getScore() << " | " << player[1].getScore() << std::endl;
}


void GameClient::run()
{
	sf::Clock clock;

	const float dt = 1.0f / 60.0f;
	float accumulator = 0.0f;

	while (GameState::Shutdown != state)
	{
		// Check for events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed :
				changeState(GameState::Shutdown);
				break;
			case sf::Event::KeyPressed :
				keyPressed(event);
				break;
			case sf::Event::KeyReleased :
				keyReleased(event);
				break;
			}
		}
		pollMessages();

		// Update the game then draw
		float frameTime = clock.restart().asSeconds();

		if (frameTime > 0.25f)
			frameTime = 0.25f;

		accumulator += frameTime;
		localTime += frameTime;
		while (accumulator >= dt)
		{
			update(dt);
			accumulator -= dt;
		}

		if (window.isOpen())
			draw();
	}
	shutdown();
} // ------------------------------------------------------------------------ //

void GameClient::pollMessages()
{
	int type;
	sf::Packet packet;
	sf::Socket::Status status = client.receive(packet);
	
	if (status == sf::Socket::Done)
	{
		packet >> type;
		if (MessageType::ClientConnected == type)
		{
			if (playerIndex == -1)	// Only if we haven't been assigned yet
			{
				packet >> playerIndex;
				changeState(GameState::WaitingForClients);
				std::cout << "Connected as client " << playerIndex << std::endl
					<< "Waiting for other clients..." << std::endl;
			}
		}
		else if (MessageType::GameStarted == type)
		{
			player[0].initialize(1);
			player[1].initialize(2);
			graphics.initialize(&window);
			changeState(GameState::Preparatory);
		}
		else if (MessageType::PlayerScored == type)
		{
			int index;
			packet >> index;
			playerScored(index);
		}
		else if (MessageType::BallReleased == type)
		{
			float angle;
			packet >> angle;
			ball.release(angle);
			changeState(GameState::Playing);
		}
		else if (MessageType::GameSync == type)
		{
			GameSyncMessage m;
			packet >> m;
			
			prevServerState = serverState;
			pssTime = ssTime;
			serverState = m.snapshot;
			ssTime = localTime;
		}
	}
	else if (status == sf::Socket::Disconnected)
	{
		std::cout << "Disconnected from server." << std::endl;
		changeState(GameState::Shutdown);
	}
}

void GameClient::draw()
{
	if (!history.empty())
	{
		window.clear();
		GameSnapshot snapshot = history.front();

		float alpha = (localTime - pssTime - 0.05f) / (ssTime - pssTime);

		for (int i = 0; i < 2; i++)
		{
			if (i != playerIndex)
				snapshot.playerPos[i] = alpha*serverState.playerPos[i] + (1-alpha)*prevServerState.playerPos[i];
		}
		snapshot.ballPos = alpha*serverState.ballPos + (1-alpha)*prevServerState.ballPos;

		if (drawGraphics)
			graphics.drawSnapshot(&snapshot);
		if (drawServerState)
			graphics.drawBounds(&serverState, sf::Color(0, 0, 255, 70));
		if (drawBounds)
			graphics.drawBounds(&snapshot, sf::Color::Red);
		window.display();
	}
} // ------------------------------------------------------------------------ //


void GameClient::keyPressed(sf::Event const &ev)
{
	if (ev.key.code == sf::Keyboard::Left)
	{
		player[playerIndex].paddle.setMoveLeft(true);
		PlayerMoveMessage m;
		m.type = MessageType::PlayerMove;
		m.right = false;
		m.value = true;
		sendMessage(&m);
	}
	else if (ev.key.code == sf::Keyboard::Right)
	{
		player[playerIndex].paddle.setMoveRight(true);
		PlayerMoveMessage m;
		m.type = MessageType::PlayerMove;
		m.right = true;
		m.value = true;
		sendMessage(&m);
	}
	else if (ev.key.code == sf::Keyboard::Num1)
		drawBounds = !drawBounds;
	else if (ev.key.code == sf::Keyboard::Num2)
		drawGraphics = !drawGraphics;
	else if (ev.key.code == sf::Keyboard::Num3)
		drawServerState = !drawServerState;
} // ------------------------------------------------------------------------ //


void GameClient::keyReleased(sf::Event const &ev)
{
	if (ev.key.code == sf::Keyboard::Left)
	{
		player[playerIndex].paddle.setMoveLeft(false);
		PlayerMoveMessage m;
		m.type = MessageType::PlayerMove;
		m.right = false;
		m.value = false;
		sendMessage(&m);
	}
	else if (ev.key.code == sf::Keyboard::Right)
	{
		player[playerIndex].paddle.setMoveRight(false);
		PlayerMoveMessage m;
		m.type = MessageType::PlayerMove;
		m.right = true;
		m.value = false;
		sendMessage(&m);
	}
} // ------------------------------------------------------------------------ //

void GameClient::sendMessage(Message const *m)
{
	sf::Packet packet;
	switch (m->type)
	{
	case MessageType::PlayerMove :
		packet << *(PlayerMoveMessage*)m;
		break;
	}

	client.send(packet);
}

void GameClient::shutdown()
{
	window.close();
} // ------------------------------------------------------------------------ //


GameClient::~GameClient()
{
} // ---------------------------------------------------------------------- //


