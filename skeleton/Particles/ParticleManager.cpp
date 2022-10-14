#include "ParticleManager.h"
#include "Particle.h"
#include "ParticleSystem/ParticleSystem.h"

ParticleManager::ParticleManager()
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

	mParticles.erase(it);
	delete* it;
	return true;
}

bool ParticleManager::Remove(vector<Particle*>::iterator& it)
{
	Particle* aux = *it;
	it = mParticles.erase(it);
	delete aux;
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
}
