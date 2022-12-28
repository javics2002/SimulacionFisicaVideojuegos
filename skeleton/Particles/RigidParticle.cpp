#include "RigidParticle.h"
#include "../Math/Lerp.h"

extern PxPhysics* gPhysics;
extern PxScene* gScene;

RigidParticle::RigidParticle(PxVec3 pos, double mass, PxShape* shape, PxVec4 color, 
	PhysicMaterial material) : color(color), startingLife(DBL_MAX), lifetime(startingLife)
{
	shape->setMaterials(&gMaterials[material], 1);
	particle = gPhysics->createRigidDynamic(PxTransform(pos));
	particle->attachShape(*shape);
	renderItem = new RenderItem(shape, particle, color);
	gScene->addActor(*particle);
	particle->setMass(mass);
}

RigidParticle::~RigidParticle()
{
	if (renderItem != nullptr) {
		DeregisterRenderItem(renderItem);
		delete renderItem;
	}
}

void RigidParticle::Integrate(double t)
{
	lifetime -= t;

	if (endColor.w != 0 && renderItem != nullptr)
		//Color interpolation
		renderItem->color = lerp(color, endColor, (startingLife - lifetime) / startingLife);
}

RigidParticle* RigidParticle::SetColor(PxVec4 col)
{
	color = col;

	if (renderItem != nullptr)
		renderItem->color = color;

	return this;
}

RigidParticle* RigidParticle::SetEndColor(PxVec4 col)
{
	endColor = col;

	return this;
}

RigidParticle* RigidParticle::SetShape(PxShape* shp)
{
	if (renderItem != nullptr)
		renderItem->shape = shp;

	return this;
}

RigidParticle* RigidParticle::SetLifetime(double life)
{
	lifetime = startingLife = life;
	return this;
}
