#pragma once
#include "IUpdateable.h"
#include "PingMath.h"

/// The Ball bounces around the stage at a constant magnitude of velocity.
class Ball : public IUpdateable
{
public:
	Vector2 position;	///< The Ball's current position
	float angle;		///< The angle/direction the Ball is travelling
	float speed;		///< The speed at which the Ball is travelling
	bool inPlay;		///< Whether the ball should be updated

	Ball(void);
	~Ball(void);
	virtual void update(float deltaTime);
};

