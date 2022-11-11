#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "../Firework.h"

ParticleSystem::ParticleSystem(ForceRegistry* fr, ParticleGenerator* generator, PxVec3 p) 
	: Particle(p, false), particles(ParticleManager(fr)), particleGenerators(ParticleManager(fr))
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

void ParticleSystem::ShootFirework(FireworkType type)
{
	particles.Add(new Firework(this, type));
}
