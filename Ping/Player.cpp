#include "Player.h"
#include "Game.h"

Player::Player()
	: number(0)
	, score(0)
{
}

void Player::initialize(int playerIndex)
{
	switch (playerIndex)
	{
	case 1:
		paddle.position = Vector2(Game::Width / 2.0f, 0);
		break;
	case 2:
		paddle.position = Vector2(Game::Width / 2.0f, (float)Game::Height);
		break;
	}

	number = playerIndex;
}

void Player::update(float deltaTime)
{
	paddle.update(deltaTime);
}

void Player::increaseScore()
{
	score++;
}

Player::~Player(void)
{
}
