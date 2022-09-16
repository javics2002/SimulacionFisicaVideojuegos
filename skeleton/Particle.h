#pragma once

#include "PxPhysicsAPI.h"
class RenderItem;

using namespace physx;

class Particle
{
	PxTransform pose;
	PxVec3 vel, acc;
	double damping;

	double inverseMass;

	PxVec4 color;
	PxShape* shape;

	RenderItem* renderItem;

public:
	Particle(PxVec3 p = { 0, 0, 0 }, PxVec3 v = {0, 0, 0}, PxVec3 a = {0, 0, 0}, double damp = .998,
		double iMass = 1, PxVec4 col = { 1, 1, 1, 1 }, PxShape* shp = nullptr);
	~Particle();
	void Integrate(double t);
};

