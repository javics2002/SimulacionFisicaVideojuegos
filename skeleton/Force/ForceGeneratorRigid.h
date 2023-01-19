#pragma once
#include "PxPhysicsAPI.h"

#define POS particle->getGlobalPose().p

using namespace std;
using namespace physx;

class RigidParticle;

enum ForceGeneratorRigidType { EXPLOSION_RIGID, SPRING_RIGID, IMPULSE_RIGID };

class ForceGeneratorRigid
{
public:
	virtual void UpdateForce(RigidParticle* p, double dt) = 0;
	virtual ~ForceGeneratorRigid() {};

	ForceGeneratorRigidType type;

	bool active = true;
};

