#include "Pinball.h"
#include "Ball.h"
#include "Flipper.h"
#include "Plunger.h"
#include "../Scene.h"

#include <iostream>

Pinball::Pinball(Scene* scene) : scene(scene)
{
	GetCamera()->SetEye(PxVec3(0, 4, 1));
	GetCamera()->SetDir(PxVec3(0, -0.8f, -0.2f));

	BuildBoard();

	ball = new Ball();
	leftFlipper = new Flipper(true);
	rightFlipper = new Flipper(false);
	plunger = new Plunger();
	scene->particles.Add(ball);
	scene->particles.Add(leftFlipper);
	scene->particles.Add(rightFlipper);
	scene->particles.Add(plunger);

	LoadHighScore();
	StartRound();

	//Falseo la inclinacion del tablero 5º para simplificar ejes
	gScene->setGravity(PxVec3(0, -cos(degToRad(5)), sin(degToRad(5))) * 9.8);
}

Pinball::~Pinball()
{
	GetCamera()->SetEye(PxVec3(50.0f, 50.0f, 50.0f));
	GetCamera()->SetDir(PxVec3(-0.6f, -0.2f, -0.7f));

	gScene->setGravity(PxVec3(0.0f, -9.8f, 0.0f));

	if (!SaveHighScore())
		cout << "Highscore couldn't be saved\n";
}

void Pinball::StartRound()
{
	ball->Reset();
	leftFlipper->Reset();
	rightFlipper->Reset();
	plunger->Reset();
}

void Pinball::ResetRound()
{
	if (!SaveHighScore())
		cout << "Highscore couldn't be saved\n";
}

void Pinball::Update(double t)
{
	leftFlipper->Update(t);
	rightFlipper->Update(t);
	plunger->Update(t);
}

void Pinball::KeyPress(unsigned char key)
{
	switch (toupper(key)) {
	case 'C': //Palanca izquierda
		leftFlipper->Flip();
		break;
	case 'N': //Palanca derecha
		rightFlipper->Flip();
		break;
	case 'M': //Lanzar bola
		plunger->Plunge();
		break;
	case 'L': //Resetear juego
		StartRound();
		break;
	default:
		break;
	}
}

void Pinball::BuildBoard()
{
	//Mesa
	scene->AddPxStatic(PxVec3(0, 1, 0), CreateShape(PxBoxGeometry(PxVec3(1.3, .7, 1.9))), 
		{ .4, .2, .4, 1 }, SOAP);

	//Paredes
	
}

void Pinball::LoadHighScore()
{
}

bool Pinball::SaveHighScore()
{
	//if(score > highScore)

	return false;
}
