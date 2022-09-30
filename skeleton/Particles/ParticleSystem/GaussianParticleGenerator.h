#pragma once

#include "ParticleGenerator.h"
#include <random>

class GaussianParticleGenerator : public ParticleGenerator
{
protected:
	PxVec3 pos, vel;
	normal_distribution<> normal{ 0,1 };

public:
	GaussianParticleGenerator(PxVec3 pos, PxVec3 vel);
	virtual vector<Particle*> generateParticles() override;
};

