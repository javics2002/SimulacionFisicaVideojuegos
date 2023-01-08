#pragma once

#include <windows.h>
#include <PxPhysicsAPI.h>
#include <vector>

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

	const int nBumpers = 6;
	const float bumperRadius = .25f, bumperSpeed = 20, 
		bumperRadiusVariation = .05f, bumperRadiusSpeed = 2,
		movingRectangleVariation = .2f, movingRectangleSpeed = 1;
	const PxVec3 bumperCenter = PxVec3(-.1, 1.74, .8),
		movingRectangleCenter = PxVec3(-.4, 1.72, .1);

	double time = 0;

	int score = 0;
	int highScore;

	void ResetRound();

	void LoadHighScore();
	bool SaveHighScore();

	void BuildBoard();

public:
	Pinball(Scene* scene);
	~Pinball();

	void Update(double t);
	void KeyPress(unsigned char key);
};

