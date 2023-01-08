#pragma once
#include <PxPhysicsAPI.h>
#include "../RenderUtils.hpp"

using namespace physx;

class RigidParticle
{
protected:
	PxVec4 color;
	PxVec4 endColor;


	double startingLife;
	double lifetime;

public:
	RigidParticle(PxVec3 pos, double mass, PxShape* shape, PxVec4 color,
		PhysicMaterial material = DEFAULT);
	~RigidParticle();

	virtual void Integrate(double t);

	RigidParticle* SetColor(PxVec4 col);
	RigidParticle* SetEndColor(PxVec4 col);
	RigidParticle* SetShape(PxShape* shp);
	RigidParticle* SetLifetime(double life);

	bool active;
	PxRigidDynamic* particle;
	RenderItem* renderItem = nullptr;
};

