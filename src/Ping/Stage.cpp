#include "Stage.h"

Stage::Stage(void)
    : width(600)
    , height(600)
    , ballRadius(10)
    , paddleSize(128, 16)
    , observer(nullptr)
{
    reset();
}


Stage::~Stage(void)
{
}

void Stage::start()
{
    ball.inPlay = true;
}

void Stage::reset()
{
    // Reset the position of the paddles and ball
    topPaddle.position = Vector2(width/2.0f, paddleSize.y/2.0f);
    topPaddle.moveLeft = false;
    topPaddle.moveRight = false;
    topPaddle.speed = 0;
    bottomPaddle.position = Vector2(width/2.0f, height - paddleSize.y/2.0f);
    bottomPaddle.moveLeft = false;
    bottomPaddle.moveRight = false;
    bottomPaddle.speed = 0;
    ball.position = Vector2(width/2.0f, height/2.0f);
    ball.inPlay = false;

    // Reset the ball angle
    do {
        // Make sure the ball initial angle is not too horizontal
        ball.angle = (std::rand() % 360) * PingMath::Pi / 180;
    }
    while (std::abs(std::cos(ball.angle)) > 0.7f);
}

void Stage::update(float deltaTime)
{
    // Update all of the entities
    topPaddle.update(deltaTime);
    bottomPaddle.update(deltaTime);
    ball.update(deltaTime);

    // Check ball collisions with edges of stage
    if (ball.position.x + ballRadius > width) { // Ball hit right side
        ball.position.x = width - ballRadius - 0.1f;
        ball.angle += PingMath::Pi - 2 * ball.angle;
    }
    else if (ball.position.x - ballRadius < 0) { // Ball hit left side
        ball.position.x = ballRadius + 0.1f;
        ball.angle += PingMath::Pi - 2 * ball.angle;
    }
    if (ball.position.y + ballRadius > height) { // Ball hit bottom
        if (observer) observer->ballHitBottom();
    }
    else if (ball.position.y - ballRadius < 0) { // Ball hit top
        if (observer) observer->ballHitTop();
    }

    // Ensure paddles don't go off stage
    if (topPaddle.position.x - paddleSize.x/2.0f < 0) {
        topPaddle.position.x = paddleSize.x/2.0f + 0.1f;
        topPaddle.speed = 0;
    }
    else if (topPaddle.position.x + paddleSize.x/2.0f > width) {
        topPaddle.position.x = width - paddleSize.x/2.0f - 0.1f;
        topPaddle.speed = 0;
    }
    if (bottomPaddle.position.x - paddleSize.x/2.0f < 0) {
        bottomPaddle.position.x = paddleSize.x/2.0f + 0.1f;
        bottomPaddle.speed = 0;
    }
    else if (bottomPaddle.position.x + paddleSize.x/2.0f > width) {
        bottomPaddle.position.x = width - paddleSize.x/2.0f - 0.1f;
        bottomPaddle.speed = 0;
    }

    // Check the collisions between the ball and the paddles
    // Top Paddle
    if (ball.position.y - ballRadius < topPaddle.position.y + paddleSize.y / 2.0f && 
        ball.position.y - ballRadius > topPaddle.position.y &&
        ball.position.x + ballRadius >= topPaddle.position.x - paddleSize.x / 2.0f &&
        ball.position.x - ballRadius <= topPaddle.position.x + paddleSize.x / 2.0f)
    {
        if (ball.position.y > topPaddle.position.y) {
            ball.angle = -ball.angle;
        }
        else {
            ball.angle = -ball.angle;
        }

        ball.position.y = topPaddle.position.y + ballRadius + paddleSize.y / 2.0f + 0.1f;
    }

    // Bottom Paddle
    if (ball.position.y + ballRadius > bottomPaddle.position.y - paddleSize.y / 2.0f &&
        ball.position.y + ballRadius < bottomPaddle.position.y &&
        ball.position.x + ballRadius >= bottomPaddle.position.x - paddleSize.x / 2.0f &&
        ball.position.x - ballRadius <= bottomPaddle.position.x + paddleSize.x / 2.0f)
    {
        if (ball.position.y > bottomPaddle.position.y) {
            ball.angle = -ball.angle;
        }
        else {
            ball.angle = -ball.angle;
        }

        ball.position.y = bottomPaddle.position.y - ballRadius - paddleSize.y / 2.0f - 0.1f;
    }
}