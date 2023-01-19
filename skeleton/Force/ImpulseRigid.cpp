#include "ImpulseRigid.h"
#include "../Particles/RigidParticle.h"

ImpulseRigid::ImpulseRigid(RigidParticle* p, PxVec3 force) : force(force)
{
	type = IMPULSE_RIGID;
	UpdateForce(p, 0);
}

ImpulseRigid::~ImpulseRigid()
{
}

void ImpulseRigid::UpdateForce(RigidParticle* p, double dt)
{
	p->particle->addForce(force);
}