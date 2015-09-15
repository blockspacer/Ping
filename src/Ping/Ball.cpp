#include "Ball.h"


Ball::Ball(void)
	: speed(390.0f)
	, inPlay(false)
{
}


Ball::~Ball(void)
{
}


void Ball::update(float deltaTime)
{
	if (inPlay)
	{
		position.x += std::cos(angle) * speed * deltaTime;
		position.y += std::sin(angle) * speed * deltaTime;
	}
}
