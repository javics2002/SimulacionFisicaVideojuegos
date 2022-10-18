#pragma once

#include <vector>
#include <queue>

using namespace std;

class Particle;

class ParticleManager
{
	vector<Particle*> mParticles;
	queue<Particle*> particleQueue;

public:
	ParticleManager();
	~ParticleManager();

	int Add(Particle* p);
	Particle* Get(int id);
	bool Remove(int id) noexcept;
	bool Remove(vector<Particle*>::iterator& it) noexcept;
	void Clear();

	void AddSafe(Particle* p) noexcept;

	void Integrate(double t);
};
