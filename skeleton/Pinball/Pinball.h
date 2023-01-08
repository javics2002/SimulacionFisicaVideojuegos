#pragma once

#include <windows.h>
#include <PxPhysicsAPI.h>
#include <unordered_map>

using namespace std;
using namespace physx;

class Ball;
class Flipper;
class RigidParticle;
class Scene;
class Plunger;

class Pinball
{
	Scene* scene = nullptr;

	Ball* ball = nullptr;
	Flipper* leftFlipper = nullptr, *rightFlipper = nullptr;
	Plunger* plunger = nullptr;
	unordered_map<PxActor*, double> obstacles;

	const int nBumpers = 6;
	const float bumperRadius = .25f, bumperSpeed = 20, 
		bumperRadiusVariation = .05f, bumperRadiusSpeed = 2,
		movingRectangleVariation = .2f, movingRectangleSpeed = 1;
	const PxVec3 bumperCenter = PxVec3(-.1, 1.74, .7),
		movingRectangleCenter = PxVec3(-.4, 1.72, .1);

	double time = 0;
	double lastContact = 0;

	//Tiempo que tiene que pasar para que dos contactos se consideren distintos
	const double contactEpsilon = .6;
	//Tiempo que tarda en resetearse el combo de contactos
	const double comboTime = 1; 
	int contactCombo;
	//Frecuencias de la escala de Do Mayor 5
	const vector<float> musicNotes = { 523.25, 587.33, 659.25, 698.46, 783.99, 880, 987.77, 1046.5 };

	int score = 0;
	int highScore = 0;

	void ResetRound();

	void LoadHighScore();
	bool SaveHighScore();

	void BuildBoard();

	void RotateBumpers();
	void MoveRectangle();

public:
	Pinball(Scene* scene);
	~Pinball();

	void Update(double t);
	void KeyPress(unsigned char key);

	void onCollision(PxActor* actor1, PxActor* actor2);
};

