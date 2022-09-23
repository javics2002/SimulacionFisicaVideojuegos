#pragma once

#include "Particle.h"
#include <vector>

using namespace std;

class Scene
{
	int mID = 0;
	vector<Particle*> mParticles;

	enum ProjectileType{ PISTOL, ARTILLERY, FIREBALL, LASER };

	struct Projectile {
		double inverseMass; //inversa de la masa
		double speed; //velocidad
		double gravity; //gravedad
		double initialHeight; //altura inicial
		double angle;
	};

	const double g = -9.8;
	const double c = 299792458;

public:
	Scene();
	~Scene();

	void LoadScene(int newID);
	void Update(double t);

	int AddParticle(Particle* p);
	Particle* GetParticle(int id);
	bool RemoveParticle(int id);
	void ClearScene();

	void KeyPress(unsigned char key, const physx::PxTransform& camera);
	void ThrowProyectile(ProjectileType type, const physx::PxTransform& camera);

	Projectile Simulate(double simulatedVel, Projectile real);
};

