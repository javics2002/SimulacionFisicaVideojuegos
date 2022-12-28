#include "Particle.h"
#include "../Math/Lerp.h"

#include "../RenderUtils.hpp"
#include <climits>
#include <iostream>
#include <cmath>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

Particle::Particle(PxVec3 p, bool visible, bool forces) : pose(PxTransform(p)), vel({ 0, 0, 0 }), acc({ 0, 0, 0 }),
	damping(.998), inverseMass(1), color({ 1, 1, 1, 1 }), endColor({ 0, 0, 0, 0 }), active(true),
	startingLife(DBL_MAX), lifetime(startingLife), checkForces(forces)
{
	shape = CreateShape(PxSphereGeometry(1.0));
	
	if(visible)
		renderItem = DBG_NEW RenderItem(shape, &pose, color);

	force = PxVec3(0);
}

Particle::Particle(Particle* p) : pose(PxTransform(PxVec3(p->pose.p))), vel(p->vel), acc(p->acc),
	damping(p->damping), inverseMass(p->damping), color(PxVec4(p->color)),
	endColor(PxVec4(p->endColor)), active(true), startingLife(p->startingLife), 
	lifetime(startingLife), shape(p->shape), checkForces(p->checkForces),
	windfriction1(p->windfriction1), windfriction2(p->windfriction2),
	integrationMethod(p->integrationMethod)
{
	renderItem = DBG_NEW RenderItem(shape, &pose, color);

	force = PxVec3(0);
}

Particle::~Particle()
{
	if (renderItem != nullptr) {
		DeregisterRenderItem(renderItem);
		delete renderItem;
	}
}

void Particle::Integrate(double t)
{	
	if (checkForces) {
		acc = force * inverseMass;
		ClearForce();
	}

	switch (integrationMethod) {
	default:
	case EULER:
		Translate(vel * t);
		vel += acc * t;
		break;
	case SEMI_IMPLICIT_EULER: 
		vel += acc * t;
		Translate(vel * t);
		break;
	}
	vel *= pow(damping, t);
	
	if (pose.p.y < 0 || pose.p.magnitudeSquared() > squaredRadius || lifetime < 0) {
		//Se sale del area util
		active = false;
		return;
	}

	lifetime -= t;

	if (endColor.w != 0 && renderItem != nullptr)
		//Color interpolation
		renderItem->color = lerp(color, endColor, (startingLife - lifetime) / startingLife);
}

Particle* Particle::Translate(PxVec3 deltaPos) {
	pose.p += deltaPos;
	return this;
}

Particle* Particle::SetPos(PxVec3 p)
{
	pose.p = p;
	return this;
}

Particle* Particle::SetVel(PxVec3 v)
{
	vel = v;
	return this;
}

Particle* Particle::SetAcc(PxVec3 a)
{
	acc = a;
	return this;
}

Particle* Particle::SetDamp(double damp)
{
	damping = damp;
	return this;
}

Particle* Particle::SetIMass(double iMass)
{
	inverseMass = iMass;
	return this;
}

double Particle::AddMass(double increment)
{
	double mass = GetMass() + increment;
	if (mass < .1)
		mass = .1;
	inverseMass = 1 / mass;
	return mass;
}

Particle* Particle::SetColor(PxVec4 col)
{
	color = col;

	if (renderItem != nullptr) 
		renderItem->color = color;

	return this;
}

Particle* Particle::SetEndColor(PxVec4 col)
{
	endColor = col;

	return this;
}

Particle* Particle::SetShape(PxShape* shp)
{
	shape = shp;
	
	if (renderItem != nullptr)
		renderItem->shape = shape;

	return this;
}

Particle* Particle::SetLifetime(double life)
{
	lifetime = startingLife = life;
	return this;
}

Particle* Particle::SetIntegrationMethod(IntegrationMethod method)
{
	integrationMethod = method;
	return this;
}

Particle* Particle::SetWindFriction1(double wf1)
{
	windfriction1 = wf1;
	return this;
}

Particle* Particle::SetWindFriction2(double wf2)
{
	windfriction2 = wf2;
	return this;
}

Particle* Particle::SetCoefRestitution(double res)
{
	coefRestitution = res;
	return this;
}

Particle* Particle::SetRadius(double r)
{
	radius = r;
	return this;
}

void Particle::AddForce(PxVec3 newForce)
{
	force += newForce;
}

void Particle::ClearForce()
{
	force *= 0;
}

PxVec3 Particle::GetPos()
{
	return pose.p;
}

PxVec3 Particle::GetVel()
{
	return vel;
}

double Particle::GetInvMass()
{
	return inverseMass;
}

double Particle::GetMass()
{
	return 1 / inverseMass;
}

double Particle::GetWindFriction1()
{
	return windfriction1;
}

double Particle::GetWindFriction2()
{
	return windfriction2;
}

bool Particle::GetCheckForces()
{
	return checkForces;
}

double Particle::GetCoefRestitution()
{
	return coefRestitution;
}

double Particle::GetRadius()
{
	return radius;
}
