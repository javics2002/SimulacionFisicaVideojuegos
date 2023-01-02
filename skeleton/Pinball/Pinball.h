#pragma once

#include <PxPhysicsAPI.h>
#include <vector>

using namespace std;
using namespace physx;

enum PinballState { START, PLAYING };

class Ball;
class Flipper;
class RigidParticle;
class Scene;

class Pinball
{
	Ball* ball = nullptr;
	Flipper* leftFlipper = nullptr, *rightFlipper = nullptr;
	vector<RigidParticle*> board;

	int score = 0;
	int highScore;

	void StartRound();
	void ResetRound();

	void LoadHighScore();
	bool SaveHighScore();

	void BuildBoard();

public:
	Pinball();
	~Pinball();

	void Update(double t);
	void KeyPress(unsigned char key);
};

