#pragma once
#include "PxPhysicsAPI.h"

using namespace std;
using namespace physx;

class Particle;

class ForceGenerator
{
public:
	virtual void UpdateForce(Particle* p, double dt) = 0;
};

