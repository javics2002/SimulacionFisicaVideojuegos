#pragma once
#include <PxPhysicsAPI.h>

class Particle;

using namespace physx;

class Contact
{
	Particle* particle[2];
	float radius[2];
	float restitution;
	PxVec3 normal;

	void ResolveVelocity(double t);
	void ResolveInterpenetration(double t);
	void Resolve(double t);

public:
	Contact(Particle* first, float radius1, Particle* second, float radius2, float restitution);
	~Contact();

	void Update(double t);
};

