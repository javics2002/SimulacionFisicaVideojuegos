#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "../Particle.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Integrate(double t)
{
	particles.Integrate(t);
	particleGenerators.Integrate(t);
}
