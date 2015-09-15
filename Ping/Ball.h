#pragma once
#include "IUpdateable.h"
#include "PingMath.h"

/// The Ball is simply a circle that moves at constant velocity.
/// When it touches the left or right side of the screen, its X-velocity is
/// negated to simulate a perfectly elastic collision. When the Ball crosses
/// the Y-position of a Player's Paddle, the other player scores, unless the
/// Ball intersects the Paddle (after which its Y-velocity is negated, and play
/// continues).
class Ball : public IUpdateable
{
public:

	// Public Members
	static const int Radius = 16;
	Vector2 position;
	Vector2 direction;
	float rotation;

	Ball(void);
	~Ball(void);
	void update(float deltaTime);

	/// Resets the Ball's position and velocity to the center of the screen and
	/// zero, respectively.
	void reset();

	/// Sets the Ball's velocity such that it travels at it's predefined speed
	/// in the direction specified by the given angle.
	/// @param	angle	The angle in radians.
	void release(float angle);

	/// Generates an appropriate angle to release the Ball.
	/// @return	The angle.
	float generateAngle();

private:
	float speed;	///< The speed at which the ball is moving in its direction
	bool inPlay;	///< Whether the Ball is in play, or is at rest
};

