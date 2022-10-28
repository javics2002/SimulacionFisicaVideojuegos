#include "ForceRegistry.h"
#include "ForceGenerator.h"
#include "../Particles/Particle.h"

void ForceRegistry::Integrate(double dt)
{
	for (auto it = begin(); it != end(); it++)
		it->first->UpdateForce(it->second, dt);
}

void ForceRegistry::AddRegistry(ForceGenerator* fg, Particle* p)
{
	insert({ fg, p });
}

void ForceRegistry::DeleteParticle(Particle* p)
{
}
