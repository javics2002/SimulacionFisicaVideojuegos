#pragma once
#include "ForceGeneratorRigid.h"

class SpringRigid : public ForceGeneratorRigid
{
protected:
	double k;
	double restLength;
	RigidParticle* other;

public:
	SpringRigid(RigidParticle* other, double k, double restLength);
	virtual ~SpringRigid() override;
	virtual void UpdateForce(RigidParticle* p, double dt) override;

	double AddK(double increment);
	double AddRestLength(double increment);
};

