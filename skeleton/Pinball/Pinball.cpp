#include "Pinball.h"
#include "Ball.h"
#include "Flipper.h"
#include <iostream>

Pinball::Pinball()
{
	ball = new Ball();
	leftFlipper = new Flipper();
	rightFlipper = new Flipper();

	LoadHighScore();
}

Pinball::~Pinball()
{
	delete ball;
	delete leftFlipper;
	delete rightFlipper;

	if (!SaveHighScore())
		cout << "Highscore couldn't be saved\n";
}

void Pinball::StartRound()
{
	ball->Reset();
	leftFlipper->Reset();
	rightFlipper->Reset();
}

void Pinball::ResetRound()
{
	if (!SaveHighScore())
		cout << "Highscore couldn't be saved\n";
}

void Pinball::Update(double t)
{
}

void Pinball::KeyPress(unsigned char key, const PxTransform& camera)
{
}

void Pinball::LoadHighScore()
{
}

bool Pinball::SaveHighScore()
{
	//if(score > highScore)

	return true;
}
