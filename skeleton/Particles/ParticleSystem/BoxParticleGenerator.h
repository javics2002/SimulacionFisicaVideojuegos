#pragma once

#include "ParticleGenerator.h"
#include <random>

class BoxParticleGenerator : public ParticleGenerator
{
	int particleNum;
	PxVec3 origin;
	PxVec3 dimensions;
	PxVec3 velocityVariation;
	double invMassVariation;

	bool firstFrame = true;

protected:
	random_device rand;
	default_random_engine generator = default_random_engine(rand());
	normal_distribution<float> normalX;
	normal_distribution<float> normalY;
	normal_distribution<float> normalZ;
	uniform_real_distribution<float> uniformX;
	uniform_real_distribution<float> uniformY;
	uniform_real_distribution<float> uniformZ;
	lognormal_distribution<> lognormalMass;

public:
	BoxParticleGenerator(Particle* p, int num, PxVec3 ori, PxVec3 dim, PxVec3 velVar, 
		double invMassVar = DBL_EPSILON, ForceRegistry* forceRegistry = nullptr);
	vector<Particle*> GenerateParticles() override;
};

