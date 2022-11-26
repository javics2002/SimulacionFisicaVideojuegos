#include "RubberBand.h"
#include "../Particles/Particle.h"

RubberBand::RubberBand(Particle* other, double k, double restLength) : Spring(other, k, restLength)
{
}

RubberBand::~RubberBand()
{
}

void RubberBand::UpdateForce(Particle* p, double dt)
{
	if (pow(restLength, 2) < (other->GetPos() - p->GetPos()).magnitudeSquared())
		Spring::UpdateForce(p, dt);
}
