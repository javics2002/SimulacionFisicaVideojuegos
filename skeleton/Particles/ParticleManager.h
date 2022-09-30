#pragma once

#include <vector>

using namespace std;

template<class T, class iterator = vector<T*>::iterator&>
class ParticleManager
{
	vector<T*> mParticles;

public:
	ParticleManager();
	~ParticleManager();

	int Add(T* p);
	T* Get(int id);
	bool Remove(int id);
	bool Remove(iterator it);
	void Clear();

	void Integrate(double t);
};
