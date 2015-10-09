#pragma once
#include "IUpdateable.h"
#include "PingMath.h"

/// Paddles are controlled by Players and are used to deflect the ball away
/// from their side of the Stage.
class Paddle : public IUpdateable
{
public:
    Vector2 position;    ///< The paddle's current position
    float speed;        ///< The speed at which the paddle is moving
    bool moveLeft;        ///< Is the paddle accelerating towards the left?
    bool moveRight;        ///< Is the paddle accelerating towards the right?

    Paddle(void);
    ~Paddle(void);
    virtual void update(float deltaTime) override;

private:
    float accel;    ///< Determines how quickly the paddle gains speed
    float decel;    ///< Determines how quickly the paddle slows down
    float topSpeed;    ///< Specifies the maximum speed the paddle can move at
};

