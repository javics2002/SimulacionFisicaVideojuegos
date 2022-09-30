#pragma once

#include "../Particle.h"
#include "PxPhysicsAPI.h"
#include <vector>

class ParticleSystem;

using namespace std;
using namespace physx;

class ParticleGenerator : public Particle
{
protected:
	Particle* prefab;
	ParticleSystem* system;

public:
	ParticleGenerator(Particle* p);
	~ParticleGenerator();

	virtual void Integrate(double t);

	virtual vector<Particle*> GenerateParticles() = 0;

	ParticleGenerator* SetSystem(ParticleSystem* s);
};

