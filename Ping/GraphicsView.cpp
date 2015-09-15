#include <sstream>
#include <iostream>
#include "GraphicsView.h"
#include "Game.h"

GraphicsView::GraphicsView(void)
	: resPath("Resources/")
{
} //--------------------------------------------------------------------------//


void GraphicsView::initialize(sf::RenderWindow *window)
{
	this->window = window;

	int style	= sf::Style::Titlebar | sf::Style::Close;
	std::string title = "Ping";

	window->create(sf::VideoMode(Game::Width, Game::Height), title, style);
	window->setKeyRepeatEnabled(false);
	window->setVerticalSyncEnabled(true);
} //--------------------------------------------------------------------------//


bool GraphicsView::loadContent()
{
	bool ok = false;

	// Load the textures
	ok = backgroundTex.loadFromFile(resPath + "background.png");
	ok = paddleTex.loadFromFile(resPath + "paddle.png");
	ok = paddleShadowTex.loadFromFile(resPath + "paddle_shadow.png");
	ok = ballTex.loadFromFile(resPath + "ball.png");
	ok = ballShadowTex.loadFromFile(resPath + "ball_shadow.png");
	ok = font.loadFromFile(resPath + "arial.ttf");

	backgroundSprite.setTexture(backgroundTex);
	ballShSprite.setTexture(ballShadowTex);
	p0ShSprite.setTexture(paddleShadowTex);
	p1ShSprite.setTexture(paddleShadowTex);
	ballSprite.setTexture(ballTex);
	p0Sprite.setTexture(paddleTex);
	p1Sprite.setTexture(paddleTex);
	
	p1ShSprite.setScale(1.0f, -1.0f);
	p1Sprite.setScale(1.0f, -1.0f);
	ballShSprite.setOrigin(32, 32);
	ballSprite.setOrigin(32, 32);
	p0ShSprite.setOrigin(128, 0);
	p0Sprite.setOrigin(128, 0);
	p1ShSprite.setOrigin(128, 0);
	p1Sprite.setOrigin(128, 0);

	return ok;
} //--------------------------------------------------------------------------//


void GraphicsView::drawSnapshot(GameSnapshot *snapshot)
{
	// Set sprite positions
	ballShSprite.setPosition(snapshot->ballPos);
	p0ShSprite.setPosition(snapshot->playerPos[0]);
	p1ShSprite.setPosition(snapshot->playerPos[1]);
	ballSprite.setPosition(snapshot->ballPos);
	p0Sprite.setPosition(snapshot->playerPos[0]);
	p1Sprite.setPosition(snapshot->playerPos[1]);

	// Draw the sprites
	window->draw(backgroundSprite);
	drawScore(snapshot->score);
	window->draw(ballShSprite);
	window->draw(ballSprite);
	window->draw(p0ShSprite);
	window->draw(p1ShSprite);
	window->draw(p0Sprite);
	window->draw(p1Sprite);

	drawCountdown(snapshot->timeUntilRelease);
} // ------------------------------------------------------------------------ //


void GraphicsView::drawBounds(GameSnapshot *snapshot, sf::Color color)
{
	// Draw paddle collision lines
	sf::RectangleShape line;
	line.setFillColor(color);
	line.setSize(sf::Vector2f((float)Paddle::Width, 4));
	line.setOrigin(116, 0);

	// p0
	line.setPosition(snapshot->playerPos[0].x, snapshot->playerPos[0].y + 50);
	window->draw(line);

	// p1
	line.setPosition(snapshot->playerPos[1].x, snapshot->playerPos[1].y - 50);
	window->draw(line);

	// Draw ball circle
	sf::CircleShape ball((float)Ball::Radius);
	ball.setFillColor(color);
	ball.setPosition(snapshot->ballPos);
	ball.setOrigin(16, 16);
	window->draw(ball);
} //--------------------------------------------------------------------------//


void GraphicsView::drawCountdown(float time)
{
	if (PingMath::between(time, 0.0f, 3.0f))
	{
		// Countdown
		int number = (int)ceil(time);

		float scale = 1.5f + (time - number);
		std::ostringstream ss;
		ss << number;

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(100);
		text.setString(ss.str());
		sf::FloatRect wh = text.getLocalBounds();
		text.setPosition(Game::Width/2.0f, Game::Height/2.0f);
		text.setOrigin(wh.width/2.0f, wh.height/2.0f);
		text.setScale(scale, scale);
		text.setColor(sf::Color(100, 100, 255));

		window->draw(text);
	}
} //--------------------------------------------------------------------------//


void GraphicsView::drawScore(int *score)
{
	const int height = 64;
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(height);
	text.setColor(sf::Color(100, 100, 100, 100));
	std::ostringstream ss;

	// Score for player 0
	ss << score[0];
	text.setPosition(8, Game::Height/2.0f - 16);
	text.setString(ss.str());
	window->draw(text);

	// Score for player 1
	ss.str("");
	ss.clear();
	ss << score[1];
	text.setPosition(8, Game::Height/2.0f - height - 2);
	text.setString(ss.str());
	window->draw(text);
} //--------------------------------------------------------------------------//


GraphicsView::~GraphicsView(void)
{
} //--------------------------------------------------------------------------//

