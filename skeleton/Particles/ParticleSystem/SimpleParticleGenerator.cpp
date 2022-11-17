#include "SimpleParticleGenerator.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

SimpleParticleGenerator::SimpleParticleGenerator(Particle* p, ForceRegistry* forceRegistry) 
	: ParticleGenerator(p, forceRegistry)
{
}

vector<Particle*> SimpleParticleGenerator::GenerateParticles()
{
	vector<Particle*> newParticles(1, DBG_NEW Particle(prefab));
	return newParticles;
}