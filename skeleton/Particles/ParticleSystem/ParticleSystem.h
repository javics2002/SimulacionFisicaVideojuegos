#pragma once

#include "../Particle.h"
#include "../ParticleManager.h"
#include "PxPhysicsAPI.h"
#include <vector>

using namespace std;
using namespace physx;

class ParticleGenerator;
enum FireworkType;

class ParticleSystem : public Particle
{
public: 
	ParticleSystem(ForceRegistry* fr = nullptr, ParticleGenerator* generator = nullptr, 
		PxVec3 p = { 0, 0, 0 });
	~ParticleSystem() override;

	void Integrate(double t) override;
	void AddGenerator(ParticleGenerator* generator);
	void ClearGenerators();
	void ReplaceGenerators(ParticleGenerator* generator);
	void ShootFirework(FireworkType type);

	ParticleManager particles;
	ParticleManager particleGenerators;
};

