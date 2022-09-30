#pragma once

#include "Particles/Projectile.h"
#include "Particles/ParticleManager.h"
#include "PxPhysicsAPI.h"

#include <random>

using namespace std;
using namespace physx;

constexpr int LAST_SCENE = 3;

class Scene
{
	int mID = 0;

	random_device rand;
	default_random_engine generator = default_random_engine(rand());

public:
	Scene();
	~Scene();

	void LoadScene(int newID);
	void Update(double t);
	void ClearScene();

	ParticleManager particles;

	void KeyPress(unsigned char key, const PxTransform& camera);
	void ThrowProyectile(ProjectileType type, const PxTransform& camera);
};

