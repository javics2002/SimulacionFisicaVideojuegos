#pragma once

#include <vector>
#include <queue>

using namespace std;

class Particle;
class RigidParticle;
class ForceRegistry;
class ForceRegistryRigid;

class ParticleManager
{
	vector<Particle*> mParticles;
	vector<RigidParticle*> mRigidParticles;
	queue<Particle*> particleQueue;
	ForceRegistry* fr;
	ForceRegistryRigid* frr;

public:
	ParticleManager(ForceRegistry* forceRegistry, 
		ForceRegistryRigid* forceRegistryRigid = nullptr);
	~ParticleManager();

	int Add(Particle* p);
	int Add(RigidParticle* p);
	Particle* Get(int id);
	RigidParticle* GetRigid(int id);
	bool Remove(vector<Particle*>::iterator& it);
	bool Remove(vector<RigidParticle*>::iterator& it);
	void Clear();
	bool Empty();
	int Size();
	int SizeRigid();
	void AddSafe(Particle* p) noexcept;

	void Integrate(double t);
};
