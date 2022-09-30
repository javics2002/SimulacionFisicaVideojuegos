#include "ParticleManager.h"

template<class T, class iterator>
ParticleManager<T, iterator>::ParticleManager()
{
}

template<class T, class iterator>
ParticleManager<T, iterator>::~ParticleManager()
{
	Clear();
}

template<class T, class iterator>
int ParticleManager<T, iterator>::Add(T* p)
{
	if (p != nullptr) {
		mParticles.push_back(p);
		return mParticles.size() - 1;
	}
	else return -1;
}

template<class T, class iterator>
T* ParticleManager<T, iterator>::Get(int id)
{
	if (id >= 0 && id < mParticles.size())
		return mParticles[id];
	else return nullptr;
}

template<class T, class iterator>
bool ParticleManager<T, iterator>::Remove(int id)
{
	if (id < 0 && id >= mParticles.size())
		return false;

	vector<T*>::iterator it = mParticles.begin();
	int i = 0;
	while (i < id) {
		it++;
		i++;
	}

	mParticles.erase(it);
	delete* it;
	return true;
}

template<class T, class iterator>
bool ParticleManager<T, iterator>::Remove(iterator it)
{
	T* aux = *it;
	it = mParticles.erase(it);
	delete aux;
	return true;
}

template<class T, class iterator>
void ParticleManager<T, iterator>::Clear()
{
	for (auto p : mParticles)
		delete p;

	mParticles.clear();
}

template<class T, class iterator>
void ParticleManager<T, iterator>::Integrate(double t)
{
	for (auto p = mParticles.begin(); p != mParticles.end(); ) {
		//Sacamos las particulas muertas de nuestro vector
		if (!(*p)->active) {
			RemoveParticle(p);
			continue;
		}

		(*p)->Integrate(t);
		p++;
	}
}
