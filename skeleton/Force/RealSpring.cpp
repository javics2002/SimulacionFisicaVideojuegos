#include "RealSpring.h"
#include "../Particles/Particle.h"

RealSpring::RealSpring(Particle* other, double k, double restLength,
	double minLength, double maxLength, double breakingLength)
	: Spring(other, k, restLength), minLength(minLength), 
	maxLength(maxLength), breakingLength(breakingLength)
{
}

RealSpring::~RealSpring()
{
}

PxVec3 sqrt(PxVec3 v) {
	return { sqrtf(v.x), sqrtf(v.y) , sqrtf(v.z) };
}

void RealSpring::UpdateForce(Particle* p, double dt)
{
	const float distanceSquared = (other->GetPos() - p->GetPos()).magnitudeSquared();

	if (distanceSquared < pow(minLength, 2)) {
		//El muelle ya no puede encogerse mas
		PxVec3 dir = (other->GetPos() - p->GetPos()).getNormalized();
		p->SetPos(other->GetPos() + dir * minLength);
	}
	else if (distanceSquared > pow(breakingLength, 2)) {
		//El muelle se rompe
		active = false;
	}
	else if (distanceSquared > pow(maxLength, 2)) {
		//El muelle se ha dado de si
		PxVec3 force = other->GetPos() - p->GetPos();
		p->AddForce(sqrt(force * (force.magnitude() - restLength) * k));
	}
	else 
		Spring::UpdateForce(p, dt);
}
