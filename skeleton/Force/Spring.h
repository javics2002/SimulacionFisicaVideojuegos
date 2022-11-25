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
	virtual ~Spring() override;
	void UpdateForce(Particle* p, double dt) override;

	double AddK(double increment);
	double AddRestLength(double increment);
};

