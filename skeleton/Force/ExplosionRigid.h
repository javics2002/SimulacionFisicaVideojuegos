#pragma once
#include "ForceGeneratorRigid.h"

class ExplosionRigid : public ForceGeneratorRigid
{
	PxVec3 center;
	double radius;
	double maxRadius;
	double force;
	double duration;
	double time;

public:
	ExplosionRigid(PxVec3 center, double radius, double force, double duration);
	~ExplosionRigid();
	void UpdateForce(RigidParticle* p, double dt) override;
};

