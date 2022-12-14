#pragma once
#include "ForceGenerator.h"

class Explosion : public ForceGenerator
{
	PxVec3 center;
	double radius;
	double maxRadius;
	double force;
	double duration;
	double time;

public:
	Explosion(PxVec3 center, double radius, double force, double duration);
	~Explosion();
	void UpdateForce(Particle* p, double dt) override;
};

