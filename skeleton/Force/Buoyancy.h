#pragma once
#include "ForceGenerator.h"

class Buoyancy : public ForceGenerator
{
	float height;
	float density;
	float volume;
	float gravity;

	Particle* liquidSurface;

public:
	Buoyancy(PxVec3 surfacePos, float height, float density, float volume, float gravity = 9.8);
	~Buoyancy() override;

	void UpdateForce(Particle* p, double dt) override;
};

