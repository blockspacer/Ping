#include "Paddle.h"


Paddle::Paddle(void)
	: position(position)
	, topSpeed(650.0f)
	, accel(80.0f)
	, decel(0.89f)
	, moveLeft(false)
	, moveRight(false)
{
}


Paddle::~Paddle(void)
{
}

void Paddle::update(float deltaTime)
{
	// Decelerate
	if (!(moveLeft || moveRight)) {
		speed *= decel;
	}
	if (!(moveLeft && moveRight)) {
		if (moveLeft) {
			speed -= accel;
		}
		else if (moveRight) {
			speed += accel;
		}
	}

	// Ensure we don't go faster than the top speed
	speed = PingMath::clamp(speed, -topSpeed, topSpeed);
	position.x += speed * deltaTime;
}