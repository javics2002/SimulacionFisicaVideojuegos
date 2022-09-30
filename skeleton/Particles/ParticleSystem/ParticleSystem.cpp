#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"

ParticleSystem::ParticleSystem(ParticleGenerator* generator, PxVec3 p) : Particle(p)
{
	if (generator == nullptr)
		AddGenerator(new GaussianParticleGenerator((new Particle(p))->SetVel({0, 30, 0})->SetAcc({ 0, -10, 0 })));
}

ParticleSystem::~ParticleSystem()
{
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
