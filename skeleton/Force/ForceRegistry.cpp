#include "ForceRegistry.h"
#include "ForceGenerator.h"
#include "../Particles/Particle.h"

void ForceRegistry::Integrate(double dt)
{
	for (auto it = begin(); it != end(); it++)
		it->first->UpdateForce(it->second, dt);
}

mapFR::iterator ForceRegistry::AddRegistry(ForceGenerator* fg, Particle* p)
{
	return insert({ fg, p });
}

void ForceRegistry::DeleteParticle(Particle* p)
{
	for (auto it = begin(); it != end();)
		if (it->second == p)
			it = erase(it);
		else
			it++;
}

void ForceRegistry::DeleteForce(ForceGenerator* fg)
{
	for (auto it = begin(); it != end();)
		if (it->first == fg)
			it = erase(it);
		else
			it++;
}
