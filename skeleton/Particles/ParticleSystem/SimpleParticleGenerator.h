#pragma once

#include "ParticleGenerator.h"

class SimpleParticleGenerator : public ParticleGenerator
{
public:
	SimpleParticleGenerator(Particle* p);
	vector<Particle*> GenerateParticles() override;
};

