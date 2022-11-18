#include "Spring.h"
#include "../Particles/Particle.h"

Spring::Spring(Particle* other, double k, double restLength)
{
}

Spring::~Spring()
{
}

void Spring::UpdateForce(Particle* p, double dt)
{
	p->AddForce(p->GetPos() - other->GetPos());
}
