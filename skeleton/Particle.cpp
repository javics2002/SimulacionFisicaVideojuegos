#include "Particle.h"

#include "RenderUtils.hpp"

Particle::Particle(PxVec3 p, PxVec3 v, PxVec3 a, double damp, double iMass, PxVec4 col, PxShape* shp) :
	pose(PxTransform(p)), vel(v), acc(a), damping(damp), inverseMass(iMass), color(col), shape(shp)
{
	if (shape == nullptr)
		shape = CreateShape(PxSphereGeometry(1.0));

	renderItem = new RenderItem(shape, &pose, color);
}

Particle::~Particle()
{
	//shape->release();
	DeregisterRenderItem(renderItem);
}

void Particle::Integrate(double t)
{
	if (inverseMass <= PX_EPS_F64)
		return;
	
	pose.p += vel * t;
	vel += acc * t;
	vel *= pow(damping, t);
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
	renderItem->color = color;
	return this;
}

Particle* Particle::SetShape(PxShape* shp)
{
	shape = shp;
	renderItem->shape = shape;
	return this;
}
