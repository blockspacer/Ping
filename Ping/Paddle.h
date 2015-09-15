#pragma once
#include "IUpdateable.h"
#include "PingMath.h"

/// Paddles are controlled by Players and are used to deflect the ball away
/// from their side of the screen.
class Paddle : public IUpdateable
{
public:

	// Public Members
	static const int Width = 230;   ///< The width of the paddle in pixels
	Vector2 position;

	// Pubic Methods
	Paddle();
	virtual void update(float deltaTime);
	virtual ~Paddle(void);

	// Accessors
	void setMoveRight(bool value) { moveRight = value; }
	void setMoveLeft(bool value) { moveLeft = value; }
	float getSpeed() const { return speed; }

protected:
	float speed;	///< The speed at which the paddle is moving
	float accel;	///< Determines how quickly the paddle gains speed
	float decel;	///< Determines how quickly the paddle slows down
	float topSpeed;	///< Specifies the maximum speed the paddle can move at
	bool moveLeft;	///< Is the paddle accelerating towards the left?
	bool moveRight;	///< Is the paddle accelerating towards the right?
};

