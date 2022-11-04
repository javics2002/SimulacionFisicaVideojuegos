#include "Particle.h"

#include "../RenderUtils.hpp"
#include <climits>
#include <iostream>
#include <cmath>

Particle::Particle(PxVec3 p, bool visible, bool forces) : pose(PxTransform(p)), vel({ 0, 0, 0 }), acc({ 0, 0, 0 }),
damping(.998), inverseMass(1), color({ 1, 1, 1, 1 }), endColor({ 0, 0, 0, 0 }), active(true),
startingLife(DBL_MAX), lifetime(startingLife), checkForces(forces)
{
	shape = CreateShape(PxSphereGeometry(1.0));
	
	if(visible)
		renderItem = new RenderItem(shape, &pose, color);

	force = PxVec3(0);
}

Particle::Particle(Particle* p) : pose(PxTransform(PxVec3(p->pose.p))), vel(p->vel), acc(p->acc),
	damping(p->damping), inverseMass(p->damping), color(PxVec4(p->color)),
	endColor(PxVec4(p->endColor)), active(true),
	startingLife(p->startingLife), lifetime(startingLife), shape(p->shape)
{
	renderItem = new RenderItem(shape, &pose, color);

	force = PxVec3(0);
}

Particle::~Particle()
{
	//shape->release();

	if (renderItem != nullptr)
		DeregisterRenderItem(renderItem);
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
		pose.p += vel * t;
		vel += acc * t;
		break;
	case RUNGE_KUTTA:
	{
		constexpr int order = 4;
		static int steps = 0;
		if (steps++ >= order) {
			int previous[order];
			break;
		}
	}
	case SEMI_IMPLICIT_EULER: 
		vel += acc * t;
		pose.p += vel * t;
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

void Particle::AddForce(PxVec3 newForce)
{
	force += newForce;
}

void Particle::ClearForce()
{
	force *= 0;
}

PxVec3 Particle::GetVel()
{
	return vel;
}

double Particle::GetInvMass()
{
	return inverseMass;
}
