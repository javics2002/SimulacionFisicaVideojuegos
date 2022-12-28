#pragma once
#include <PxPhysicsAPI.h>

class Particle;

using namespace physx;

class Contact
{
	void ResolveVelocity(float t);
	void ResolveInterpenetration(float t);

protected:
	void Resolve(float t);

public:
	Particle* particle[2];
	float restitution, penetration;
	PxVec3 normal;
};

