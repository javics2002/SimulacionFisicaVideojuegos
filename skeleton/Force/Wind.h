#pragma once
#include "ForceGenerator.h"

class Wind : public ForceGenerator
{
	PxVec3 windVelocity;

	PxVec3 Pow(PxVec3 v, double e);

public:
	Wind(PxVec3 windVel);
	~Wind();
	void UpdateForce(Particle* p, double dt) override;
};

