#pragma once
#include "ForceGenerator.h"

class Spring : public ForceGenerator
{
protected:
	double k;
	double restLength;
	Particle* other;

public:
	Spring(Particle* other, double k, double restLength);
	~Spring();
	void UpdateForce(Particle* p, double dt) override;

	bool AddK(double increment);
	bool AddRestLength(double increment);
};

