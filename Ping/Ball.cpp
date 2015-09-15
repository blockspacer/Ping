#include "Ball.h"
#include "Game.h"

Ball::Ball(void)
	: speed(500.0f)
{
	reset();
} //--------------------------------------------------------------------------//


void Ball::update(float deltaTime)
{
	Vector2 rot(position.x - Game::Width / 2.0f, position.y - Game::Height / 2.0f);
	rotation = atan2(rot.x, rot.y);

	if (inPlay)
	{
		position += speed * direction * deltaTime;

		// Bounce off left and right sides
		if (position.x + Radius > Game::Width)
		{
			position.x = Game::Width - Radius - 1.0f;
			direction.x = -direction.x;
		}
		else if (position.x - Radius < 0)
		{
			position.x = Radius + 1.0f;
			direction.x = -direction.x;
		}
	}
} //--------------------------------------------------------------------------//


void Ball::reset()
{
	inPlay = false;
	position.x = Game::Width / 2.0f;
	position.y = Game::Height / 2.0f;
	direction.x = 0.0f;
	direction.y = 0.0f;
} //--------------------------------------------------------------------------//


void Ball::release(float angle)
{
	direction.x = cos(angle);
	direction.y = sin(angle);
	inPlay = true;
} //--------------------------------------------------------------------------//


float Ball::generateAngle()
{
	float angle = 0.0f;

	if (PingMath::random(0, 1) > 0.5)
	{
		// Send toward top
		angle = PingMath::PiOver2 + PingMath::random(-PingMath::PiOver4, PingMath::PiOver4);
	}
	else
	{
		// Send toward bottom
		angle = -PingMath::PiOver2 + PingMath::random(-PingMath::PiOver4, PingMath::PiOver4);
	}

	return angle;
} //--------------------------------------------------------------------------//


Ball::~Ball(void)
{
} //--------------------------------------------------------------------------//
