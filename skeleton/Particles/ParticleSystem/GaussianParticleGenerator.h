#pragma once

#include "ParticleGenerator.h"
#include <random>

class GaussianParticleGenerator : public ParticleGenerator
{
protected:
	random_device rand;
	default_random_engine generator = default_random_engine(rand());
	normal_distribution<> normal;

public:
	GaussianParticleGenerator(Particle* p, double mean = 0, double deviation = 1);
	vector<Particle*> GenerateParticles() override;
};

