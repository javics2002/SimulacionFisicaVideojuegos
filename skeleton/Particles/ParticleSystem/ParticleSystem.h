#pragma once

#include "PxPhysicsAPI.h"
#include "../ParticleManager.h"
#include <vector>

using namespace std;
using namespace physx;

class Particle;
class ParticleGenerator;

class ParticleSystem
{
	double lifetime;

	PxVec3 gravity;

public: 
	ParticleSystem();
	~ParticleSystem();

	ParticleManager<Particle> particles;
	ParticleManager<ParticleGenerator> particleGenerators;

	void Integrate(double t);
};

