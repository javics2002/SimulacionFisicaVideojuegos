#pragma once

#include "ParticleGenerator.h"

class FireworkGenerator : public ParticleGenerator
{
public:
	FireworkGenerator();
	~FireworkGenerator();

	void Integrate(double t) override;

	vector<Particle*> GenerateParticles() override;
};

