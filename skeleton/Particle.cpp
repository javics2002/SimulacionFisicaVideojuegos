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
