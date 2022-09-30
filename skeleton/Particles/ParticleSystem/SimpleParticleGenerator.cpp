#include "SimpleParticleGenerator.h"

SimpleParticleGenerator::SimpleParticleGenerator(Particle* p) : ParticleGenerator(p)
{
}

vector<Particle*> SimpleParticleGenerator::GenerateParticles()
{
	vector<Particle*> newParticles(1, new Particle(prefab));
	return newParticles;
}