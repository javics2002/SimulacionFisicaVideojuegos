#pragma once

#include <vector>
#include <queue>


using namespace std;

class Particle;
class ForceRegistry;

class ParticleManager
{
	vector<Particle*> mParticles;
	queue<Particle*> particleQueue;
	ForceRegistry* fr;

public:
	ParticleManager(ForceRegistry* forceRegistry);
	~ParticleManager();

	int Add(Particle* p);
	Particle* Get(int id);
	bool Remove(int id);
	bool Remove(vector<Particle*>::iterator& it);
	void Clear();

	void AddSafe(Particle* p) noexcept;

	void Integrate(double t);
};
