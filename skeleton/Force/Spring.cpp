#include "Spring.h"
#include "../Particles/Particle.h"

Spring::Spring(Particle* other, double k, double restLength) : other(other), k(k), restLength(restLength)
{
	type = SPRING;
}

Spring::~Spring()
{
}

void Spring::UpdateForce(Particle* p, double dt)
{
	PxVec3 force = other->GetPos() - p->GetPos();
	p->AddForce(force * (force.magnitude() - restLength) * k);
}

double Spring::AddK(double increment)
{
	k += increment;

	if (k < 1)
		k = 1;

	return k;
}

double Spring::AddRestLength(double increment)
{
	restLength += increment;

	if (restLength < DBL_EPSILON)
		restLength = DBL_EPSILON;

	return restLength;
}
