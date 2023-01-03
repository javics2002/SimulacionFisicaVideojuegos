#pragma once

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
	Scene* scene;
	Ball* ball = nullptr;
	Flipper* leftFlipper = nullptr, *rightFlipper = nullptr;
	Plunger* plunger = nullptr;
	vector<RigidParticle*> board;

	int score = 0;
	int highScore;

	void StartRound();
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

