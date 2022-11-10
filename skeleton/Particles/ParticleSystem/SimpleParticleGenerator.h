#pragma once

#include "ParticleGenerator.h"

class SimpleParticleGenerator : public ParticleGenerator
{
public:
	SimpleParticleGenerator(Particle* p, ForceRegistry* forceRegistry = nullptr);
	vector<Particle*> GenerateParticles() override;
};

