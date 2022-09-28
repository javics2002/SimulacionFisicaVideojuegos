#pragma once

#include "Particle.h"
#include <vector>
#include <random>

using namespace std;

constexpr int LAST_SCENE = 2;

class Scene
{
	int mID = 0;
	vector<Particle*> mParticles;

	enum ProjectileType{ PISTOL, ARTILLERY, FIREBALL, LASER, GOLF, FRISBEE, BALLOON };

	struct Projectile {
		double inverseMass; //inversa de la masa
		double speed; //velocidad
		double gravity; //gravedad
		double initialHeight; //altura inicial
		double angle;
		double variation;
	};

	const double g = -9.8;
	const double c = 299792458;

	random_device rand;
	default_random_engine generator = default_random_engine(rand());

public:
	Scene();
	~Scene();

	void LoadScene(int newID);
	void Update(double t);

	int AddParticle(Particle* p);
	Particle* GetParticle(int id);
	bool RemoveParticle(int id);
	bool RemoveParticle(vector<Particle*>::iterator& it);
	void ClearScene();

	void KeyPress(unsigned char key, const physx::PxTransform& camera);
	void ThrowProyectile(ProjectileType type, const physx::PxTransform& camera);

	Projectile Simulate(double simulatedVel, Projectile real);
};

