#pragma once

#include "PxPhysicsAPI.h"
#include <vector>

using namespace std;
using namespace physx;

class Particle;

class ParticleGenerator //Heredar de particle
{
public:
	ParticleGenerator();
	~ParticleGenerator();

	virtual vector<Particle*> generateParticles() = 0;

};

