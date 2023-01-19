#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "../Firework.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

ParticleSystem::ParticleSystem(ForceRegistry* fr, ParticleGenerator* generator, 
	PxVec3 p) : Particle(p, false), 
	particles(ParticleManager(fr)), particleGenerators(ParticleManager(fr))
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

void ParticleSystem::ShootFirework(FireworkType type, PxVec3 spawnPos)
{
	particles.Add(DBG_NEW Firework(this, type, spawnPos));
}
