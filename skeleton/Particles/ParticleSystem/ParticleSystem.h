#pragma once

#include "../Particle.h"
#include "../ParticleManager.h"
#include "PxPhysicsAPI.h"
#include <vector>

using namespace std;
using namespace physx;

class ParticleGenerator;

class ParticleSystem : public Particle
{
public: 
	ParticleSystem(ParticleGenerator* generator = nullptr, PxVec3 p = { 0, 0, 0 });
	~ParticleSystem();

	void Integrate(double t) override;
	void AddGenerator(ParticleGenerator* generator);

	ParticleManager particles;
	ParticleManager particleGenerators;
};

