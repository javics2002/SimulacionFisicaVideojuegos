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

	double Distance(Particle* p, PxVec3 point);
	PxVec3 Velocity(PxVec3 pos);
	PxVec3 Pow(PxVec3 v, double e);

public:
	Explosion(PxVec3 center, double radius, double force, double duration);
	~Explosion();
	void UpdateForce(Particle* p, double dt) override;
};

