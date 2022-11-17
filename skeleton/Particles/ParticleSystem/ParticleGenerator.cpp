#include "ParticleGenerator.h"
#include "ParticleSystem.h"
#include "../../Force/ForceRegistry.h"

ParticleGenerator::ParticleGenerator(Particle* p, ForceRegistry* forceRegistry) 
	: Particle({0, 0, 0}, false), prefab(p), fr(forceRegistry)
{
}

ParticleGenerator::~ParticleGenerator()
{
	delete prefab;

	fg.clear();
}

void ParticleGenerator::Integrate(double t)
{
	Particle::Integrate(t);

	if (active && system != nullptr) {
		vector<Particle*> newParticles = GenerateParticles();

		for (auto p : newParticles) {
			system->particles.Add(p);
			for (auto f : fg)
				fr->AddRegistry(f.get(), p);
		}
	}
}

ParticleGenerator* ParticleGenerator::SetSystem(ParticleSystem* s)
{
	system = s;
	return this;
}

ParticleGenerator* ParticleGenerator::AddForceGenerator(shared_ptr<ForceGenerator> forceGenerator)
{
	fg.push_back(forceGenerator);
	
	return this;
}
