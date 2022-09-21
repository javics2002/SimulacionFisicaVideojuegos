#pragma once

#include "Particle.h"
#include <vector>

using namespace std;

class Scene
{
	int mID = 0;
	vector<Particle*> mParticles;

public:
	Scene();
	~Scene();

	void LoadScene(int newID);
	void Update(double t);

	int AddParticle(Particle* p);
	Particle* GetParticle(int id);
	bool RemoveParticle(int id);
	void ClearScene();
};

