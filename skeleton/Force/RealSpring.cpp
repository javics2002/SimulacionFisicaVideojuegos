#include "RealSpring.h"
#include "../Particles/Particle.h"
#include <iostream>

RealSpring::RealSpring(Particle* other, double k, double restLength,
	double minLength, double maxLength, double breakingLength)
	: Spring(other, k, restLength), minLength(minLength), 
	maxLength(maxLength), breakingLength(breakingLength), deformed(false)
{
}

RealSpring::~RealSpring()
{
}

float ssqrtf(float f) {
	if (f >= 0)
		return sqrtf(f);
	else
		return -sqrtf(-f);
}

//signed sqrt. mantiene el signo
PxVec3 ssqrt(PxVec3 v) {
	return { ssqrtf(v.x), ssqrtf(v.y) , ssqrtf(v.z) };
}

void RealSpring::UpdateForce(Particle* p, double dt)
{
	if (!active)
		return;

	const float distanceSquared = (other->GetPos() - p->GetPos()).magnitudeSquared();

	if (distanceSquared == 0)
		cout << "xd\n";
	if (distanceSquared < pow(minLength, 2)) {
		//El muelle ya no puede encogerse mas
		PxVec3 dir = (p->GetPos() - other->GetPos()).getNormalized();
		p->SetPos(other->GetPos() + dir * minLength);
		p->SetVel(PxVec3(0));
		cout << "Tope\n";
	}
	else if (distanceSquared > pow(breakingLength, 2)) {
		//El muelle se rompe
		active = false;
		cout << "Muelle roto\n";
		return;
	}
	else if (!deformed && distanceSquared > pow(maxLength, 2)) {
		//El muelle se ha dado de si
		deformed = true;
		cout << "Muelle deformado\n";
	}
	
	if (deformed) {
		PxVec3 force = other->GetPos() - p->GetPos();
		force = ssqrt(force * (force.magnitude() - restLength) * k);
		p->AddForce(force);
	}
	else
		Spring::UpdateForce(p, dt);
}
