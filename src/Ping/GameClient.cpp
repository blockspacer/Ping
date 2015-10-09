#include <iostream>
#include <sstream>
#include "GameClient.h"
#include "GameSnapshot.h"

GameClient::GameClient(void)
{
    predict = true;        // Use prediction to reduce the effects of lag
    interpolate = true;    // Interpolate for smooth animation
    serverAddr = sf::IpAddress("127.0.0.1");
    playerIndex = 0;

    //===========================//
    // Graphical representations //
    //===========================//
    // Create the top paddle
    topPaddle.setSize(stage.paddleSize);
    topPaddle.setOutlineColor(sf::Color::Black);
    topPaddle.setFillColor(sf::Color(255, 255, 255));
    topPaddle.setOrigin(stage.paddleSize / 2.f);

    // Create the bottom paddle
    bottomPaddle.setSize(stage.paddleSize);
    bottomPaddle.setOutlineColor(sf::Color::Black);
    bottomPaddle.setFillColor(sf::Color(255, 255, 255));
    bottomPaddle.setOrigin(stage.paddleSize / 2.f);

    // Create the ball
    ball.setRadius(stage.ballRadius);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(stage.ballRadius / 2, stage.ballRadius / 2);
}


// Load font and connect to the server
bool GameClient::initialize()
{
    if (!font.loadFromFile("arial.ttf")) {
        return false;
    }

    std::cout << "Connecting to " <<
        serverAddr.toString() << "..." << std::endl;

    if (server.connect(serverAddr, 44000) != sf::Socket::Done) {
        return false;
    }
    server.setBlocking(false);

    return true;
}


void GameClient::shutdown()
{
    window.close();
}


void GameClient::update(float deltaTime)
{
    pollMessages();
    handleInput();

    countdown -= deltaTime;

    //===========================//
    // Client-side interpolation //
    //===========================//
    float alpha;
    if (interpolate && state != GameState::Preparatory) {
        alpha = (localTime - psTime - 0.05f) / (ssTime - psTime);
    }
    else {
        alpha = 1.0f;
    }

    for (int i = 0; i < 2; i++) {
        if (i != playerIndex || !predict) {
            player[i].paddle->position = alpha*serverState.paddlePos[i] + (1-alpha)*prevServerState.paddlePos[i];
        }
    }
    stage.ball.position = alpha*serverState.ballPos + (1-alpha)*prevServerState.ballPos;

    //========================//
    // Client-side prediction //
    //========================//
    if (predict) {
        stage.update(deltaTime);
    }
}


void GameClient::draw()
{
    // Clear the display to black
    window.clear(sf::Color(0, 0, 0));

    // Set positions of the sprites
    topPaddle.setPosition(stage.topPaddle.position);
    bottomPaddle.setPosition(stage.bottomPaddle.position);
    ball.setPosition(stage.ball.position);

    // Draw the paddles and the ball
    window.draw(topPaddle);
    window.draw(bottomPaddle);
    window.draw(ball);
    drawCountdown();
    drawScore();

    window.display();
}


void GameClient::handleInput()
{
    sf::Event event;
    window.pollEvent(event);
    // Window closed or escape key pressed: exit
    if ((event.type == sf::Event::Closed) ||
        ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
        state = GameState::Shutdown;
    }

    // Toggle prediction
    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)) {
        predict = !predict;
        std::cout << "Client-side prediction: " << predict << std::endl;
    }

    // Toggle interpolation
    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::I)) {
        interpolate = !interpolate;
        std::cout << "Client-side interpolation: " << interpolate << std::endl;
    }

    // Move the player's paddle and send a message to the server
    if (GameState::Playing == state) {
        sf::Packet p;
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                player[playerIndex].paddle->moveLeft = true;
                p << Message::PlayerMoveLeft;
                p << true;
                server.send(p);
            }
            else if (event.key.code == sf::Keyboard::Right) {
                player[playerIndex].paddle->moveRight = true;
                p << Message::PlayerMoveRight;
                p << true;
                server.send(p);
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Left) {
                player[playerIndex].paddle->moveLeft = false;
                p << Message::PlayerMoveLeft;
                p << false;
                server.send(p);
            }
            else if (event.key.code == sf::Keyboard::Right) {
                player[playerIndex].paddle->moveRight = false;
                p << Message::PlayerMoveRight;
                p << false;
                server.send(p);
            }
        }
    }
}


void GameClient::pollMessages()
{
    int type;
    sf::Packet packet;
    sf::Socket::Status status = server.receive(packet);
    
    if (status == sf::Socket::Done) {
        // Always expect the type first
        packet >> type;

        if (Message::ClientConnected == type) {
            packet >> playerIndex;    // Server assigns us a playerIndex
            std::cout << "Connected as client " << playerIndex << std::endl
                << "Waiting for other clients..." << std::endl;
            state = GameState::WaitingForClients;
        }
        else if (Message::GameStarted == type) {
            openDisplay();
            state = GameState::Preparatory;
        }
        else if (Message::PlayerScored == type) {
            int index;
            packet >> index;
            player[index].score++;
            stage.reset();
            state = GameState::Preparatory;
            countdown = 3.0f;
        }
        else if (Message::BallReleased == type) {
            packet >> stage.ball.angle;
            stage.start();
            state = GameState::Playing;
        }
        else if (Message::GameSync == type) {
            GameSnapshot gs;
            packet >> gs;

            prevServerState = serverState;
            psTime = ssTime;
            serverState = gs;
            ssTime = localTime;
        }
    }
    else if (status == sf::Socket::Disconnected) {
        std::cout << "Disconnected from server." << std::endl;
        state = GameState::Shutdown;
    }
}


void GameClient::openDisplay()
{
    int style = sf::Style::Titlebar | sf::Style::Close;
    std::string title = "Ping";
    window.create(
        sf::VideoMode((unsigned int)stage.width, (unsigned int)stage.height),
        title,
        style);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(true);
}


void GameClient::drawCountdown()
{
    if (PingMath::between(countdown, 0.0f, 3.0f)) {
        // Show whole number only
        int number = (int)ceil(countdown);

        float scale = 1.5f + (countdown - number);
        std::ostringstream ss;
        ss << number;

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(100);
        text.setString(ss.str());

        sf::FloatRect wh = text.getLocalBounds();
        text.setPosition(stage.width/2.0f, stage.height/2.0f);
        text.setOrigin(wh.width/2.0f, wh.height/2.0f);
        text.setScale(scale, scale);
        text.setColor(sf::Color(200, 200, 200));

        window.draw(text);
    }
} //--------------------------------------------------------------------------//


void GameClient::drawScore()
{
    const int height = 64;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(height);
    text.setColor(sf::Color(150, 150, 150, 150));
    std::ostringstream ss;

    // Score for player 0
    ss << player[0].score;
    text.setPosition(8, stage.height/2.0f - height - 2);
    text.setString(ss.str());
    window.draw(text);

    // Score for player 1
    ss.str("");
    ss.clear();
    ss << player[1].score;
    text.setPosition(8, stage.height/2.0f - 16);
    text.setString(ss.str());
    window.draw(text);
} //--------------------------------------------------------------------------//


GameClient::~GameClient(void)
{
}