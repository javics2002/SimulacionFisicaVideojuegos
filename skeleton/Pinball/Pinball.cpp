#include "Pinball.h"
#include "Ball.h"
#include "Flipper.h"
#include <iostream>

Pinball::Pinball()
{
	GetCamera()->SetEye(PxVec3(0, 2, 0));
	GetCamera()->SetDir(PxVec3(0, -0.8f, -0.2f));

	BuildBoard();

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

	GetCamera()->SetEye(PxVec3(50.0f, 50.0f, 50.0f));
	GetCamera()->SetDir(PxVec3(-0.6f, -0.2f, -0.7f));

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

void Pinball::KeyPress(unsigned char key)
{
}

void Pinball::BuildBoard()
{
	//scene->AddPxStatic(PxVec3(40), CreateShape(PxBoxGeometry(PxVec3(1.3, .7, 1.9))), { .97, .85, .97, 1 });
}

void Pinball::LoadHighScore()
{
}

bool Pinball::SaveHighScore()
{
	//if(score > highScore)

	return false;
}
