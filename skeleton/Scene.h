#pragma once

#include "Particles/Particle.h"
#include "Particles/Projectile.h"
#include "Particles/ParticleManager.h"

#include <random>

using namespace std;

constexpr int LAST_SCENE = 2;

class Scene
{
	int mID = 0;

	const double g = -9.8;
	const double c = 299792458;

	random_device rand;
	default_random_engine generator = default_random_engine(rand());

public:
	Scene();
	~Scene();

	void LoadScene(int newID);
	void Update(double t);
	void ClearScene();

	ParticleManager<Particle> particles;

	void KeyPress(unsigned char key, const physx::PxTransform& camera);
	void ThrowProyectile(ProjectileType type, const physx::PxTransform& camera);
};

