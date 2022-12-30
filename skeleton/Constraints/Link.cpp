#include "Link.h"
#include "../Particles/Particle.h"

float Link::CurrentLength() const
{
	return (particle[0]->GetPos() - particle[1]->GetPos()).magnitude();
}

Link::Link(Particle* first, Particle* second)
{
	particle[0] = first;
	particle[1] = second;
}

Link::~Link()
{
}
