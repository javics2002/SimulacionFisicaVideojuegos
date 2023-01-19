#pragma once
#include "../Particles/RigidParticle.h"

class ParticleSystem;
class ParticleGenerator;
class Scene;

class Ball : public RigidParticle
{
	const PxVec3 startPos = { 1.1, 1.72, 1.1 };

	ParticleGenerator* trail;

	bool gamer = false;
	bool tornado = false;

public:
	Ball(Scene* scene);
	~Ball() override;

	void Reset();
	void Integrate(double t);

	ParticleSystem* system;

	void ToggleGamer();
	void ToggleTornado();
};

