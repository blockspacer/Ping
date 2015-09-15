#pragma once
#include "IUpdateable.h"
#include "Paddle.h"

class Player :
	public IUpdateable
{
public:
	Player();
	void initialize(int playerNumber);
	virtual ~Player(void);

	virtual void update(float deltaTime);

	Paddle paddle;

	void increaseScore();

	int getScore() const
	{
		return this->score;
	}

	int getNumber() const
	{
		return this->number;
	}

private:
	int score;
	int number;
};

