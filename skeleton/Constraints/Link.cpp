#include "Link.h"
#include "../Particles/Particle.h"

float Link::CurrentLength() const
{
	return (particle[0]->GetPos() - particle[1]->GetPos()).magnitude();
}
