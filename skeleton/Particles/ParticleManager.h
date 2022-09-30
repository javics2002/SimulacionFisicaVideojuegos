#pragma once

#include <vector>

using namespace std;

class Particle;

class ParticleManager
{
	vector<Particle*> mParticles;

public:
	ParticleManager();
	~ParticleManager();

	int Add(Particle* p);
	Particle* Get(int id);
	bool Remove(int id);
	bool Remove(vector<Particle*>::iterator& it);
	void Clear();

	void Integrate(double t);
};
