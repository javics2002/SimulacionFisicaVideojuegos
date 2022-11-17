#include "GaussianParticleGenerator.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

GaussianParticleGenerator::GaussianParticleGenerator(Particle* p, double mean, double deviation,
	ForceRegistry* forceRegistry) : ParticleGenerator(p, forceRegistry)
{
	normal = normal_distribution<>(mean, deviation);
}

vector<Particle*> GaussianParticleGenerator::GenerateParticles()
{
	PxVec3 vel = prefab->GetVel();
	vel = PxVec3(vel.x + normal(generator), vel.y + normal(generator), vel.z + normal(generator));
	vector<Particle*> newParticles(1, (DBG_NEW Particle(prefab))->SetVel(vel));
	return newParticles;
}