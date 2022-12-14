#pragma once
#include "PxPhysicsAPI.h"

using namespace std;
using namespace physx;

class Particle;

enum ForceGeneratorType { GRAVITY, DRAG, WIND, WHIRLWIND, EXPLOSION, SPRING, IMPULSE, BUOYANCY };

class ForceGenerator
{
public:
	virtual void UpdateForce(Particle* p, double dt) = 0;
	virtual ~ForceGenerator() {};

	ForceGeneratorType type;

	bool active = true;
};

