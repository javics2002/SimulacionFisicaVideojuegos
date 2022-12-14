#pragma once

#include "ExplosionRigid.h"
#include "SpringRigid.h"

#include <unordered_map>
#include <climits>

using namespace std;

class RigidParticle;
typedef unordered_multimap<ForceGeneratorRigid*, RigidParticle*> mapFRR;

class ForceRegistryRigid : public mapFRR
{
public:
	void Integrate(double dt);
	mapFRR::iterator AddRegistry(ForceGeneratorRigid* fg, RigidParticle* p);
	void DeleteParticle(RigidParticle* p);
	void DeleteForce(ForceGeneratorRigid* fg);
};

