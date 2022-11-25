#include "Particle.h"

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
	windfriction1(p->windfriction1), windfriction2(p->windfriction2)
{
	renderItem = DBG_NEW RenderItem(shape, &pose, color);

	force = PxVec3(0);
}

Particle::~Particle()
{
	//shape->release();

	if (renderItem != nullptr) {
		DeregisterRenderItem(renderItem);
		delete renderItem;
	}
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

PxVec4 lerp(PxVec4 a, PxVec4 b, float f) {
	return { lerp(a.x, b.x, f), lerp(a.y, b.y, f), lerp(a.z, b.z, f), lerp(a.w, b.w, f) };
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
	{
		Integration euler = EulerIntegrate(t);
		pose.p += euler.position;
		vel += euler.velocity;
	}
		break;
	case SEMI_IMPLICIT_EULER: 
	{
		Integration semiImplicitEuler = SemiImplicitEulerIntegrate(t);
		pose.p += semiImplicitEuler.position;
		vel += semiImplicitEuler.velocity;
	}
		break;
	case RUNGE_KUTTA:
	{
		Integration semiImplicitEuler = RungeKuttaIntegrate(t);
		pose.p += semiImplicitEuler.position;
		vel += semiImplicitEuler.velocity;
	}
	}
	vel *= pow(damping, t);
	cout << "Vel = " << vel.magnitude() << "\n";
	
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

Integration Particle::EulerIntegrate(double t) {
	Integration i;
	i.position = vel * t;
	i.velocity = acc * t;
	return i;
}

Integration Particle::SemiImplicitEulerIntegrate(double t) {
	Integration i;
	i.velocity = acc * t;
	i.position = vel + i.velocity * t;
	return i;
}

Integration Particle::RungeKuttaIntegrate(double t) {
	rungekutta[0] = SemiImplicitEulerIntegrate(0);
	rungekutta[1] = SemiImplicitEulerIntegrate(t / 2);
	rungekutta[1].position + t * rungekutta[0].position * .5;
	rungekutta[1].velocity + t * rungekutta[0].velocity * .5;
	rungekutta[2] = SemiImplicitEulerIntegrate(t / 2);
	rungekutta[2].position + t * rungekutta[1].position * .5;
	rungekutta[2].velocity + t * rungekutta[1].velocity * .5;
	rungekutta[3] = SemiImplicitEulerIntegrate(t);
	rungekutta[3].position + t * rungekutta[2].position;
	rungekutta[3].velocity + t * rungekutta[2].velocity;

	Integration i;
	i.position = (rungekutta[0].position + 2 * rungekutta[1].position + 2 * rungekutta[2].position 
		+ rungekutta[3].position) * t / 6;
	i.velocity = (rungekutta[0].velocity + 2 * rungekutta[1].velocity + 2 * rungekutta[2].velocity
		+ rungekutta[3].velocity) * t / 6;
	return i;
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
