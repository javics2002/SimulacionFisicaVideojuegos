#pragma once

#include "PxPhysicsAPI.h"

class RenderItem;

using namespace std;
using namespace physx;

enum IntegrationMethod { EULER, SEMI_IMPLICIT_EULER, RUNGE_KUTTA };

//Cuando la particula se aleje de este radio, morira
constexpr double squaredRadius = 1000000;

class Particle
{
protected:
	PxTransform pose;
	PxVec3 vel, acc;
	PxVec3 force;
	double damping;

	double inverseMass;

	PxVec4 color;
	PxVec4 endColor;
	PxShape* shape = nullptr;

	RenderItem* renderItem = nullptr;

	double startingLife;
	double lifetime;

	IntegrationMethod integrationMethod = EULER;
	bool checkForces = false;

public:
	Particle(PxVec3 p = { 0, 0, 0 }, bool visible = true, bool forces = false);
	Particle(Particle* p);
	virtual ~Particle();
	virtual void Integrate(double t);

	Particle* SetPos(PxVec3 p);
	Particle* SetVel(PxVec3 v);
	Particle* SetAcc(PxVec3 a);
	Particle* SetDamp(double damp);
	Particle* SetIMass(double iMass);
	Particle* SetColor(PxVec4 col);
	Particle* SetEndColor(PxVec4 col);
	Particle* SetShape(PxShape* shp);
	Particle* SetLifetime(double life);
	Particle* SetIntegrationMethod(IntegrationMethod method);

	void AddForce(PxVec3 newForce);
	void ClearForce();

	PxVec3 GetVel();
	double GetInvMass();

	bool active;
};

