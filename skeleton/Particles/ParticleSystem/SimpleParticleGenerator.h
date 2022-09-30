#pragma once

#include "ParticleGenerator.h"

class SimpleParticleGenerator : public ParticleGenerator
{
public:
	SimpleParticleGenerator(PxVec3 pos, PxVec3 vel, double iMass);
	virtual vector<Particle*> generateParticles() override;
};

