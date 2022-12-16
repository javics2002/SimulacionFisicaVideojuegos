#pragma once

#include "PxPhysicsAPI.h"

using namespace std;
using namespace physx;

class RenderItem;

enum IntegrationMethod { EULER, SEMI_IMPLICIT_EULER };

//Cuando la particula se aleje de este radio, morira
constexpr double squaredRadius = 10000000;

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

	double windfriction1 = 1;
	double windfriction2 = 0;

	double coefRestitution;

public:
	Particle(PxVec3 p = { 0, 0, 0 }, bool visible = true, bool forces = false);
	Particle(Particle* p);
	virtual ~Particle();
	virtual void Integrate(double t);

	Particle* Translate(PxVec3 deltaPos);

	Particle* SetPos(PxVec3 p);
	Particle* SetVel(PxVec3 v);
	Particle* SetAcc(PxVec3 a);
	Particle* SetDamp(double damp);
	Particle* SetIMass(double iMass);
	double AddMass(double increment);
	Particle* SetColor(PxVec4 col);
	Particle* SetEndColor(PxVec4 col);
	Particle* SetShape(PxShape* shp);
	Particle* SetLifetime(double life);
	Particle* SetIntegrationMethod(IntegrationMethod method);
	Particle* SetWindFriction1(double wf1);
	Particle* SetWindFriction2(double wf2);

	void AddForce(PxVec3 newForce);
	void ClearForce();

	PxVec3 GetPos();
	PxVec3 GetVel();
	double GetInvMass();
	double GetMass();
	double GetWindFriction1();
	double GetWindFriction2();
	bool GetCheckForces();
	double GetCoefRestitution();

	bool active;
};

