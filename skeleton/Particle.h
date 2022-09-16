#pragma once

#include "PxPhysicsAPI.h"
class RenderItem;

using namespace physx;

class Particle
{
	PxTransform pose;
	PxVec3 vel, acc;
	double damp;

	PxVec4 color;
	PxShape* shape;

	RenderItem* renderItem;

public:
	Particle(PxVec3 p, PxVec3 v, PxVec3 a, double damping, PxVec4 col, PxShape* shp);
	Particle(PxVec3 p, PxVec3 v, PxVec3 a, double damping);
	~Particle();
	void Integrate(double t);
};

