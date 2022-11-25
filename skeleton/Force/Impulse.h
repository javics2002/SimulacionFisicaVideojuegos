#pragma once
#include "ForceGenerator.h"
class Impulse : public ForceGenerator
{
	PxVec3 force;

public:
	Impulse(Particle* p, PxVec3 force);
	~Impulse();
	void UpdateForce(Particle* p, double dt) override;
};

