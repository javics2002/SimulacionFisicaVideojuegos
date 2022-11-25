#include "Impulse.h"
#include "../Particles/Particle.h"

Impulse::Impulse(Particle* p, PxVec3 force) : force(force)
{
	UpdateForce(p, 0);
}

Impulse::~Impulse()
{
}

void Impulse::UpdateForce(Particle* p, double dt)
{
	p->AddForce(force);
	
}
