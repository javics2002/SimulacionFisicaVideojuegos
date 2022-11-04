#pragma once

#include "../Particle.h"
#include "PxPhysicsAPI.h"
#include <vector>

class ParticleSystem;
class ForceRegistry;
class ForceGenerator;

using namespace std;
using namespace physx;

class ParticleGenerator : public Particle
{
protected:
	Particle* prefab;
	ParticleSystem* system;

	ForceRegistry* fr;
	vector<ForceGenerator*> fg;

public:
	ParticleGenerator(Particle* p, ForceRegistry* forceRegistry = nullptr);
	~ParticleGenerator();

	virtual void Integrate(double t);

	virtual vector<Particle*> GenerateParticles() = 0;

	ParticleGenerator* SetSystem(ParticleSystem* s);
	ParticleGenerator* AddForceGenerator(ForceGenerator* forceGenerator);
};

