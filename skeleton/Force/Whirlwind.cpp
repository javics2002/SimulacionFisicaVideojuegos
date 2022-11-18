#include "Whirlwind.h"
#include "../Particles/Particle.h"

Whirlwind::Whirlwind(PxVec3 center, double radius, double force) : center(center),
	radius(radius), force(force)
{
}

Whirlwind::~Whirlwind()
{
}

void Whirlwind::UpdateForce(Particle* p, double dt)
{
	if (p != nullptr && p->GetInvMass() > DBL_EPSILON)
		p->AddForce(p->GetWindFriction1() * force * Velocity(p->GetPos())
			+ p->GetWindFriction2() * Pow(force * Velocity(p->GetPos()), 2));
}

PxVec3 Whirlwind::Velocity(PxVec3 pos)
{
	PxVec3 v = pos - center;
	v = 1 * v.cross(PxVec3(0, 1, 0)) + 5 * -v;
	v.y = 0;
	return v;
}

PxVec3 Whirlwind::Pow(PxVec3 v, double e)
{
	return PxVec3(pow(v.x, e), pow(v.y, e), pow(v.z, e));
}