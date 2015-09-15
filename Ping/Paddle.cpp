#include "Paddle.h"
#include "Game.h"

Paddle::Paddle()
	: topSpeed(800.0f)
	, accel(100.0f)
	, decel(0.9f)
	, moveLeft(false)
	, moveRight(false)
{
} //--------------------------------------------------------------------------//


Paddle::~Paddle(void)
{
} //--------------------------------------------------------------------------//


void Paddle::update(float deltaTime)
{
	// Decelerate
	if (!(moveLeft || moveRight))
	{
		speed *= decel;
	}
	if (!(moveLeft && moveRight))
	{
		if (moveLeft)
		{
			//if (speed > 0) speed *= decel;
			speed -= accel;
		}
		else if (moveRight)
		{
			//if (speed < 0) speed *= decel;
			speed += accel;
		}
	}

	// Ensure we don't go faster than the top speed
	speed = PingMath::clamp(speed, -topSpeed, topSpeed);
	position.x += speed * deltaTime;

	// Prevent us from going off screen
	if (position.x + Width / 2.0f > Game::Width)
	{
		position.x = Game::Width - 230.0 / 2.0f - 1.0f;
		speed = 0;
	}
	else if (position.x - Width / 2.0f < 0)
	{
		position.x = Width / 2.0f + 1.0f;
		speed = 0;
	}
} //--------------------------------------------------------------------------//

