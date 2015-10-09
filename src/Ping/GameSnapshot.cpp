#include "GameSnapshot.h"
#include "Game.h"

GameSnapshot::GameSnapshot(Game const *game)
{
    for (int i = 0; i < 2; i++) {
        score[i] = game->player[i].score;
        paddlePos[i] = game->player[i].paddle->position;
    }

    ballPos = game->stage.ball.position;
}


GameSnapshot::~GameSnapshot(void)
{
}