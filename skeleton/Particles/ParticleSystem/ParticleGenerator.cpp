#include "ParticleGenerator.h"
#include "ParticleSystem.h"

ParticleGenerator::ParticleGenerator(Particle* p) : Particle(), prefab(p)
{
}

ParticleGenerator::~ParticleGenerator()
{
	delete prefab;
}

void ParticleGenerator::Integrate(double t)
{
	Particle::Integrate(t);

	if (active && system != nullptr) {
		vector<Particle*> newParticles = GenerateParticles();

		for (auto p : newParticles)
			system->particles.Add(p);
	}
}

ParticleGenerator* ParticleGenerator::SetSystem(ParticleSystem* s)
{
	system = s;
	return this;
}
