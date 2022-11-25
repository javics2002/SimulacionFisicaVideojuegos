#include "Spring.h"
#include "../Particles/Particle.h"

Spring::Spring(Particle* other, double k, double restLength) : other(other), k(k), restLength(restLength)
{
}

Spring::~Spring()
{
}

void Spring::UpdateForce(Particle* p, double dt)
{
	PxVec3 force = other->GetPos() - p->GetPos();
	p->AddForce(force * (force.magnitude() - restLength) * k);
}

bool Spring::AddK(double increment)
{
	k += increment;

	if (k < DBL_EPSILON) {
		k = DBL_EPSILON;
		return false;
	}

	return true;
}

bool Spring::AddRestLength(double increment)
{
	restLength += increment;

	if (restLength < DBL_EPSILON) {
		restLength = DBL_EPSILON;
		return false;
	}

	return true;
}
