#pragma once

#include <PxPhysicsAPI.h>
#include <vector>

using namespace std;
using namespace physx;

enum PinballState { START, PLAYING };

class Ball;
class Flipper;
class RigidParticle;

class Pinball
{
	Ball* ball = nullptr;
	Flipper* leftFlipper = nullptr, *rightFlipper = nullptr;
	vector<RigidParticle*> board;

	int score = 0;
	int highScore;

public:
	Pinball();
	~Pinball();

	void StartRound();
	void ResetRound();
	void Update(double t);
	void KeyPress(unsigned char key, const PxTransform& camera);

	void LoadHighScore();
	bool SaveHighScore();
};

