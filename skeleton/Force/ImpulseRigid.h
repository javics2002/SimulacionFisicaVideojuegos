#pragma once
#include "ForceGeneratorRigid.h"

class ImpulseRigid : public ForceGeneratorRigid
{
	PxVec3 force;

public:
	ImpulseRigid(RigidParticle* p, PxVec3 force);
	~ImpulseRigid();
	void UpdateForce(RigidParticle* p, double dt) override;
};

