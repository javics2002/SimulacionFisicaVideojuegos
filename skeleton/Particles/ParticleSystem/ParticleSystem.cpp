#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"

ParticleSystem::ParticleSystem(ParticleGenerator* generator, PxVec3 p) : Particle(p, false)
{
	if (generator != nullptr)
		AddGenerator(generator);
}

ParticleSystem::~ParticleSystem()
{
	ClearGenerators();
	particles.Clear();
}

void ParticleSystem::Integrate(double t)
{
	Particle::Integrate(t);

	particles.Integrate(t);
	particleGenerators.Integrate(t);
}

void ParticleSystem::AddGenerator(ParticleGenerator* generator)
{
	generator->SetSystem(this);
	particleGenerators.Add(generator);
}

void ParticleSystem::ClearGenerators()
{
	particleGenerators.Clear();
}

void ParticleSystem::ReplaceGenerators(ParticleGenerator* generator)
{
	ClearGenerators();
	AddGenerator(generator);
}
