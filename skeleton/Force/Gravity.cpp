#include "Gravity.h"
#include "../Particles/Particle.h"

Gravity::Gravity(PxVec3 gravity) : g(gravity)
{
	type = GRAVITY;
}

Gravity::~Gravity()
{
}

void Gravity::UpdateForce(Particle* p, double dt)
{
	if(p != nullptr && p->GetInvMass() > DBL_EPSILON)
		p->AddForce(g / p->GetInvMass());
}
