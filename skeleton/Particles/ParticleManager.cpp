#include "ParticleManager.h"
#include "Particle.h"
#include "ParticleSystem/ParticleSystem.h"
#include "../Force/ForceRegistry.h"

ParticleManager::ParticleManager(ForceRegistry* forceRegistry) : fr(forceRegistry)
{
}

ParticleManager::~ParticleManager()
{
	Clear();
}

int ParticleManager::Add(Particle* p)
{
	if (p != nullptr) {
		mParticles.push_back(p);
		return mParticles.size() - 1;
	}
	else return -1;
}

Particle* ParticleManager::Get(int id)
{
	if (id >= 0 && id < mParticles.size())
		return mParticles[id];
	else return nullptr;
}

bool ParticleManager::Remove(int id)
{
	if (id < 0 && id >= mParticles.size())
		return false;

	vector<Particle*>::iterator it = mParticles.begin();
	int i = 0;
	while (i < id) {
		it++;
		i++;
	}

	if ((*it)->GetCheckForces())
		fr->DeleteParticle(*it);
	Particle* p = *it;
	mParticles.erase(it);
	delete p;
	return true;
}

bool ParticleManager::Remove(vector<Particle*>::iterator& it)
{
	Particle* p = *it;
	if((*it)->GetCheckForces())
		fr->DeleteParticle(*it);
	it = mParticles.erase(it);
	delete p;
	return true;
}

void ParticleManager::Clear()
{
	/*ParticleSystem* system = nullptr;
	if (!mParticles.empty()) {
		system = dynamic_cast<ParticleSystem*>(*mParticles.begin());
		delete system;
	}
	*/
	
	for (auto p : mParticles)
		delete p;

	mParticles.clear();

	while (particleQueue.size() > 0) {
		Particle *p = particleQueue.front();
		particleQueue.pop();
		delete p;
	}
}

void ParticleManager::AddSafe(Particle* p) noexcept
{
	if(p != nullptr)
		particleQueue.push(p);
}

void ParticleManager::Integrate(double t)
{
	for (auto p = mParticles.begin(); p != mParticles.end(); ) {
		//Sacamos las particulas muertas de nuestro vector
		if (!(*p)->active) {
			Remove(p);
			continue;
		}

		(*p)->Integrate(t);
		p++;
	}

	while (particleQueue.size() > 0) {
		mParticles.push_back(particleQueue.front());
		particleQueue.pop();
	}
}
