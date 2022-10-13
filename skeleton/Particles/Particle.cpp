#include "Particle.h"

#include "../RenderUtils.hpp"
#include <climits>
#include <iostream>

Particle::Particle(PxVec3 p, bool visible) : pose(PxTransform(p)), vel({ 0, 0, 0 }), acc({ 0, 0, 0 }),
	damping(.998), inverseMass(1), color({ 1, 1, 1, 1 }), active(true), 
	startingLife(DBL_MAX), lifetime(startingLife)
{
	shape = CreateShape(PxSphereGeometry(1.0));
	
	if(visible)
		renderItem = new RenderItem(shape, &pose, color);
}

Particle::Particle(Particle* p) : pose(PxTransform(PxVec3(p->pose.p))), vel(p->vel), acc(p->acc),
	damping(p->damping), inverseMass(p->damping), color(PxVec4(p->color)), active(true), 
	startingLife(p->startingLife), lifetime(startingLife), shape(p->shape)
{
	renderItem = new RenderItem(shape, &pose, color);
}

Particle::~Particle()
{
	//shape->release();

	if (renderItem != nullptr)
		DeregisterRenderItem(renderItem);
}

void Particle::Integrate(double t)
{
	if (inverseMass <= PX_EPS_F64)
		return;
	
	pose.p += vel * t;
	vel += acc * t;
	vel *= pow(damping, t);

	if (pose.p.y < 0 || pose.p.magnitudeSquared() > squaredRadius || lifetime < 0) {
		active = false;
		return;
	}

	lifetime -= t;
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

PxVec3 Particle::GetVel()
{
	return vel;
}
