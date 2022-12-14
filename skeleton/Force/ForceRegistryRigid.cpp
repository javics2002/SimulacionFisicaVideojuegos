#include "ForceRegistryRigid.h"

void ForceRegistryRigid::Integrate(double dt)
{
	for (auto it = begin(); it != end(); it++)
		it->first->UpdateForce(it->second, dt);

	for (auto it = begin(); it != end();)
		if (!it->first->active) {
			DeleteForce(it->first);
			it = begin();
		}
		else
			it++;
}

mapFRR::iterator ForceRegistryRigid::AddRegistry(ForceGeneratorRigid* fg, RigidParticle* p)
{
	return insert({ fg, p });
}

void ForceRegistryRigid::DeleteParticle(RigidParticle* p)
{
	for (auto it = begin(); it != end();)
		if (it->second == p)
			it = erase(it);
		else
			it++;
}

void ForceRegistryRigid::DeleteForce(ForceGeneratorRigid* fg)
{
	for (auto it = begin(); it != end();)
		if (it->first == fg)
			it = erase(it);
		else
			it++;
}
