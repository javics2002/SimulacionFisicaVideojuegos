#include "Particle.h"

#include "RenderUtils.hpp"

Particle::Particle(PxVec3 p, PxVec3 v, PxVec3 a, double damping, PxVec4 col = { 1, 1, 1, 1 }, PxShape* shp = CreateShape(PxSphereGeometry(1.0))) :
	pose(PxTransform(p)), vel(v), acc(a), damp(damping), color(col), shape(shp), renderItem(new RenderItem(shape, &pose, color))
{
}

Particle::Particle(PxVec3 p, PxVec3 v, PxVec3 a, double damping) : pose(PxTransform(p)), vel(v), acc(a), damp(damping)
{
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0)), &pose, { 1, 1, 1, 1 });
}

Particle::~Particle()
{
	//shape->release();
	DeregisterRenderItem(renderItem);
}

void Particle::Integrate(double t)
{
	pose = PxTransform(pose.p.x + vel.x * t, pose.p.y + vel.y * t, pose.p.z + vel.z * t);
}
