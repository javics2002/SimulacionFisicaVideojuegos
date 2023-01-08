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
	ResetRound();

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

	cout << "Score: " << score << "\n";
	cout << "Highscore: " << highScore << "\n";
}

void Pinball::ResetRound()
{
	ball->Reset();
	leftFlipper->Reset();
	rightFlipper->Reset();
	plunger->Reset();

	if (!SaveHighScore())
		cout << "Highscore couldn't be saved\n";

	cout << "Score: " << score << "\n";
	cout << "Highscore: " << highScore << "\n";
}

void Pinball::Update(double t)
{
	leftFlipper->Update(t);
	rightFlipper->Update(t);
	plunger->Update(t);

	RotateBumpers();
	MoveRectangle();

	time += t;
}

void Pinball::KeyPress(unsigned char key)
{
	switch (toupper(key)) {
	case 'M': //Lanzar bola
		plunger->Plunge();
		break;
	case 'L': //Resetear juego
		ResetRound();
		break;
	default:
		break;
	}
}

void Pinball::RotateBumpers()
{
	for (int i = 0; i < nBumpers; i++) {
		float angle = degToRad(i * 360.0f / nBumpers + time * bumperSpeed);
		scene->particles.GetRigid(i)->particle
			->setKinematicTarget(PxTransform(PxVec3(bumperCenter.x - sin(angle) *
				(bumperRadius + sin(time * bumperRadiusSpeed) * bumperRadiusVariation),
				bumperCenter.y, bumperCenter.z - cos(angle) *
				(bumperRadius + sin(time * bumperRadiusSpeed) * bumperRadiusVariation))));
	}
}

void Pinball::MoveRectangle()
{
	scene->particles.GetRigid(nBumpers)->particle
		->setKinematicTarget(PxTransform(PxVec3(movingRectangleCenter.x
			+ sin(time * movingRectangleSpeed) * movingRectangleVariation,
			movingRectangleCenter.y, movingRectangleCenter.z), PxQuat(degToRad(5), PxVec3(0, 1, 0))));
}

void Pinball::onCollision(PxActor* actor1, PxActor* actor2)
{
	if (actor1 == ball->particle && obstacles.count(actor2) && obstacles[actor2] + contactEpsilon < time) {
		cout << "Ultima vez que colisione " << time - obstacles[actor2] << "\n";
		score += 100;
		//Beep(1046.5, 300);
		obstacles[actor2] = time;
	}
}

void Pinball::BuildBoard()
{
	//Obstaculos
	PxRigidStatic* superbounceIzquierdo = scene->AddPxStatic(PxVec3(0), //Superbounce izquierdo
		CreateShape(PxBoxGeometry(PxVec3(.225, .04, .05))), { .08, .04, .08, 1 }, SUPERBOUNCY);
	superbounceIzquierdo->setGlobalPose(PxTransform(PxVec3(-.8, 1.72, 1.2), 
		PxQuat(degToRad(-75), PxVec3(0, 1, 0))));
	obstacles.insert({ superbounceIzquierdo, 0 });

	PxRigidStatic* superbounceDerecho = scene->AddPxStatic(PxVec3(0), //Superbounce derecho
		CreateShape(PxBoxGeometry(PxVec3(.225, .04, .05))), { .08, .04, .08, 1 }, SUPERBOUNCY);
	superbounceDerecho->setGlobalPose(PxTransform(PxVec3(.6, 1.72, 1.2), 
		PxQuat(degToRad(75), PxVec3(0, 1, 0))));
	obstacles.insert({ superbounceDerecho, 0 });

	PxRigidStatic* topeIzquierdo = scene->AddPxStatic(PxVec3(0), //Tope izquierdo
		CreateShape(PxBoxGeometry(PxVec3(.04))), { .08, .04, .08, 1 }, SUPERBOUNCY);
	topeIzquierdo->setGlobalPose(PxTransform(PxVec3(-1.15, 1.72, .5), 
		PxQuat(degToRad(45), PxVec3(0, 1, 0))));
	obstacles.insert({ topeIzquierdo, 0 });

	PxRigidStatic* topeDerecho = scene->AddPxStatic(PxVec3(0), //Tope derecho
		CreateShape(PxBoxGeometry(PxVec3(.04))), { .08, .04, .08, 1 }, SUPERBOUNCY);
	topeDerecho->setGlobalPose(PxTransform(PxVec3(.95, 1.72, .5), 
		PxQuat(degToRad(45), PxVec3(0, 1, 0))));
	obstacles.insert({ topeDerecho, 0 });

	obstacles.insert({ scene->AddPxStatic(PxVec3(0, 1.75, -0.4), //Esfera 1
		CreateShape(PxSphereGeometry(0.07)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(-0.3, 1.75, -0.4), //Esfera 2
		CreateShape(PxSphereGeometry(0.07)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(-.6, 1.75, -0.4), //Esfera 3
		CreateShape(PxSphereGeometry(0.07)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(-.9, 1.75, -0.4), //Esfera 4
		CreateShape(PxSphereGeometry(0.07)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(.7, 1.8, -0.6), //Esfera 5
		CreateShape(PxSphereGeometry(0.05)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(.7, 1.8, -0.3), //Esfera 6
		CreateShape(PxSphereGeometry(0.05)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(.7, 1.8, 0), //Esfera 7
		CreateShape(PxSphereGeometry(0.05)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(.4, 1.8, -0.75), //Esfera 8
		CreateShape(PxSphereGeometry(0.05)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(.4, 1.8, -0.45), //Esfera 9
		CreateShape(PxSphereGeometry(0.05)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(.4, 1.8, -.15), //Esfera 10
		CreateShape(PxSphereGeometry(0.05)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	obstacles.insert({ scene->AddPxStatic(PxVec3(.7, 1.8, -0.9), //Esfera 11
		CreateShape(PxSphereGeometry(0.05)), { 1, 0.5, 0.5, 1 }, SUPERBOUNCY), 0 });
	
	//Obstaculos cinematicos
	for (int i = 0; i < nBumpers; i++) { //Bumpers
		float angle = degToRad(i * 360.0f / nBumpers);
		RigidParticle* bumper = new RigidParticle(PxVec3(bumperCenter.x - sin(angle) * bumperRadius,
			bumperCenter.y, bumperCenter.z - cos(angle) * bumperRadius), .2,
			CreateShape(PxSphereGeometry(0.05)), PxVec4(.5, .2, 0.7, 1), SUPERBOUNCY);
		bumper->particle->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		obstacles.insert({bumper->particle, 0});
		scene->particles.Add(bumper);
	}
	RigidParticle* movingRectangle = new RigidParticle(movingRectangleCenter, .2, //Rectangulo movil
		CreateShape(PxBoxGeometry(PxVec3(.1, .04, .04))), PxVec4(.2, 0, 0.7, 1), SUPERBOUNCY);
	movingRectangle->particle->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	obstacles.insert({ movingRectangle->particle, 0 });
	scene->particles.Add(movingRectangle);

	//Mesa
	scene->AddPxStatic(PxVec3(0, 1, 0), CreateShape(PxBoxGeometry(PxVec3(1.3, .7, 1.9))), 
		{ .4, .2, .4, 1 }, WALL);

	//Paredes
	scene->AddPxStatic(PxVec3(1.25, 1.75, 0), //Derecha
		CreateShape(PxBoxGeometry(PxVec3(.1, .1, 1.9))), { .08, .04, .08, 1 }, WALL);
	scene->AddPxStatic(PxVec3(1, 1.72, .8), //Derecha auxiliar
		CreateShape(PxBoxGeometry(PxVec3(.05, .04, 1))), { .04, .02, .04, 1 }, WALL);
	scene->AddPxStatic(PxVec3(0, 1.75, -1.13), //Arriba
		CreateShape(PxBoxGeometry(PxVec3(1.3, .1, .05))), { .08, .04, .08, 1 }, WALL);
	scene->AddPxStatic(PxVec3(-1.25, 1.75, 0), //Izquierda
		CreateShape(PxBoxGeometry(PxVec3(.1, .1, 1.9))), { .08, .04, .08, 1 }, WALL);
	scene->AddPxStatic(PxVec3(0), //Esquina superior derecha
		CreateShape(PxBoxGeometry(PxVec3(.1, .1, .4))), { .08, .04, .08, 1 }, WALL)
		->setGlobalPose(PxTransform(PxVec3(1.1, 1.75, -1.1), PxQuat(degToRad(45), PxVec3(0, 1, 0))));
	scene->AddPxStatic(PxVec3(.6, 1.755, -.5), //Parte alta
		CreateShape(PxBoxGeometry(PxVec3(.4, .01, .6))), { .004, .002, .004, 1 }, WALL);
	scene->AddPxStatic(PxVec3(0), //Rampa
		CreateShape(PxBoxGeometry(PxVec3(.1, .01, .08))), { .08, .04, .08, 1 }, WALL)
		->setGlobalPose(PxTransform(PxVec3(.4, 1.725, .15), PxQuat(degToRad(35), PxVec3(1, -.3, 0).getNormalized())));
	scene->AddPxStatic(PxVec3(1, 1.8, -.5), //Derecha alta
		CreateShape(PxBoxGeometry(PxVec3(.05, .08, .6))), { .04, .02, .04, 1 }, WALL);
}

void Pinball::LoadHighScore()
{
}

bool Pinball::SaveHighScore()
{
	if (score > highScore)
		highScore = score;

	return false;
}
