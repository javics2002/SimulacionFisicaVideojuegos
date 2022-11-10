#include "Wind.h"
#include "../Particles/Particle.h"
#include <math.h>

Wind::Wind(PxVec3 windVel) : windVelocity(windVel)
{
}

Wind::~Wind()
{
}

void Wind::UpdateForce(Particle* p, double dt)
{
	if (p != nullptr && p->GetInvMass() > DBL_EPSILON)
		p->AddForce(p->GetWindFriction1() * (windVelocity - p->GetVel()) 
			+ p->GetWindFriction2() * Pow(windVelocity - p->GetVel(), 2));
}

PxVec3 Wind::Pow(PxVec3 v, double e) {
	return PxVec3(pow(v.x, e), pow(v.y, e), pow(v.z, e));
}
