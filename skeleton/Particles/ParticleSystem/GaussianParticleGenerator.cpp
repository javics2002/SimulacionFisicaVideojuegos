#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(Particle* p, double mean, double deviation,
	ForceRegistry* forceRegistry) : ParticleGenerator(p, forceRegistry)
{
	normal = normal_distribution<>(mean, deviation);
}

vector<Particle*> GaussianParticleGenerator::GenerateParticles()
{
	PxVec3 vel = prefab->GetVel();
	vel = PxVec3(vel.x + normal(generator), vel.y + normal(generator), vel.z + normal(generator));
	vector<Particle*> newParticles(1, (new Particle(prefab))->SetVel(vel));
	return newParticles;
}