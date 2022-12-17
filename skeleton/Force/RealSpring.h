#pragma once
#include "Spring.h"

class RealSpring : public Spring
{
protected:
	double minLength, maxLength, breakingLength;

public:
	RealSpring(Particle* other, double k, double restLength,
		double minLength, double maxLength, double breakingLength);
	~RealSpring();

	void UpdateForce(Particle* p, double dt) override;
};

