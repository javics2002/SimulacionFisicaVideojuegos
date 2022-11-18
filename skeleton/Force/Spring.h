#pragma once
#include "ForceGenerator.h"

class Spring : public ForceGenerator
{
	double k;
	double restLength;
	Particle* other;

public:
	Spring(Particle* other, double k, double restLength);
	~Spring();
	void UpdateForce(Particle* p, double dt) override;
};

