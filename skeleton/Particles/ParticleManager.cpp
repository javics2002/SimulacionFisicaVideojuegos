#include "ParticleManager.h"
#include "Particle.h"
#include "RigidParticle.h"
#include "ParticleSystem/ParticleSystem.h"
#include "../Force/ForceRegistry.h"
#include "../Force/ForceRegistryRigid.h"

ParticleManager::ParticleManager(ForceRegistry* forceRegistry, 
	ForceRegistryRigid* forceRegistryRigid) : fr(forceRegistry), frr(forceRegistryRigid)
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

int ParticleManager::Add(RigidParticle* p)
{
	if (p != nullptr ) {
		mRigidParticles.push_back(p);
		return mRigidParticles.size() - 1;
	}
	else return -1;
}

Particle* ParticleManager::Get(int id)
{
	if (id >= 0 && id < mParticles.size())
		return mParticles[id];
	else return nullptr;
}

RigidParticle* ParticleManager::GetRigid(int id)
{
	if (id >= 0 && id < mRigidParticles.size())
		return mRigidParticles[id];
	else return nullptr;
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

bool ParticleManager::Remove(vector<RigidParticle*>::iterator& it)
{
	RigidParticle* p = *it;
	if(frr != nullptr)
		frr->DeleteParticle(*it);
	it = mRigidParticles.erase(it);
	delete p;
	return true;
}

void ParticleManager::Clear()
{
	for (auto p : mParticles)
		delete p;

	mParticles.clear();
	
	for (auto p : mRigidParticles)
		delete p;

	mRigidParticles.clear();

	while (particleQueue.size() > 0) {
		Particle *p = particleQueue.front();
		particleQueue.pop();
		delete p;
	}
}

bool ParticleManager::Empty()
{
	return mParticles.empty() && mRigidParticles.empty();
}

int ParticleManager::Size()
{
	return mParticles.size();
}

int ParticleManager::SizeRigid()
{
	return  mRigidParticles.size();
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

	for (auto p = mRigidParticles.begin(); p != mRigidParticles.end(); ) {
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
