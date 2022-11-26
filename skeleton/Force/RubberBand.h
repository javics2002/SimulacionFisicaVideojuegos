#pragma once
#include "Spring.h"

class RubberBand : public Spring
{
public:
	RubberBand(Particle* other, double k, double restLength);
	~RubberBand();
	void UpdateForce(Particle* p, double dt) override;
};

