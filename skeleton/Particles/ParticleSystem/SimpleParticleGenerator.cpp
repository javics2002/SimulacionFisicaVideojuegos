#include "SimpleParticleGenerator.h"

SimpleParticleGenerator::SimpleParticleGenerator(Particle* p, ForceRegistry* forceRegistry) 
	: ParticleGenerator(p, forceRegistry)
{
}

vector<Particle*> SimpleParticleGenerator::GenerateParticles()
{
	vector<Particle*> newParticles(1, new Particle(prefab));
	return newParticles;
}