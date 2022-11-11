#pragma once
#include "ForceGenerator.h"

class Whirlwind : public ForceGenerator
{
	PxVec3 center;
	double radius;
	double force;

	PxVec3 Velocity(PxVec3 pos);
	PxVec3 Pow(PxVec3 v, double e);

public:
	Whirlwind(PxVec3 center, double radius, double force);
	~Whirlwind();
	void UpdateForce(Particle* p, double dt) override;
};

