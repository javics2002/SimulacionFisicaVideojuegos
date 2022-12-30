#pragma once
#include <PxPhysicsAPI.h>

class Particle;

using namespace physx;

class Link
{
protected:
	Particle* particle[2];
	
	float CurrentLength() const;

public:
	Link(Particle* first, Particle* second);
	~Link();

	virtual void Update(double t) = 0;
};

