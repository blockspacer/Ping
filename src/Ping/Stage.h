#pragma once
#include "IUpdateable.h"
#include "Paddle.h"
#include "Ball.h"

/// An object that should receive notifications from a Stage
class StageObserver
{
public:

	/// Called when the Ball has hit the top of the Stage
	virtual void ballHitTop() = 0;

	/// Called when the Ball has hit the top of the Stage
	virtual void ballHitBottom() = 0;
};


/// The Stage is the class that holds the simulation, including the paddles
/// and ball.
class Stage : public IUpdateable
{
public:
	const float width;
	const float height;
	const Vector2 paddleSize;
	const float ballRadius;

	Paddle topPaddle;
	Paddle bottomPaddle;
	Ball ball;

	Stage(void);
	~Stage(void);
	virtual void update(float deltaTime);

	/// Set the ball to being in play.
	void start();

	/// Resets the ball and paddle positions and speeds. The ball is no longer
	/// in play after calling this method.
	void reset();

	/// Sets an observer.
	/// @param [in]	o	The object that should receive notifications from this
	/// 				stage.
	void setObserver(StageObserver* o) { observer = o; }

private:
	StageObserver *observer;
};

