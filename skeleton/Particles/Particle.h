#pragma once

#include "PxPhysicsAPI.h"
class RenderItem;

using namespace physx;

//Cuando la particula se aleje de este radio, morira
constexpr double squaredRadius = 1000000;

class Particle
{
	PxTransform pose;
	PxVec3 vel, acc;
	double damping;

	double inverseMass;

	PxVec4 color;
	PxShape* shape = nullptr;

	RenderItem* renderItem = nullptr;

	double startingLife;
	double lifetime;

public:
	Particle(PxVec3 p = { 0, 0, 0 }, bool visible = true);
	Particle(Particle* p);
	~Particle();
	virtual void Integrate(double t);

	Particle* SetVel(PxVec3 v);
	Particle* SetAcc(PxVec3 a);
	Particle* SetDamp(double damp);
	Particle* SetIMass(double iMass);
	Particle* SetColor(PxVec4 col);
	Particle* SetShape(PxShape* shp);
	Particle* SetLifetime(double life);

	PxVec3 GetVel();

	bool active;
};

