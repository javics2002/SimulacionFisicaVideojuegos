#include "Plunger.h"
#include "../Math/Lerp.h"

double Plunger::LerpValue()
{
    return 1 - tweenTime / plungeTime;
}

Plunger::Plunger() : RigidParticle(startPos, .3, CreateShape(PxBoxGeometry(PxVec3(.04, .036, .3))),
	{ .01, .01, .01, 1 }, METAL)
{
	particle->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	tweenTime = 0;
}

Plunger::~Plunger()
{
}

void Plunger::Plunge()
{
	pulling = !pulling;

	if (pulling) {
		pulledPos = startPos;
	}
	else {
		tweenTime = plungeTime;
	}
}

void Plunger::Reset()
{
	pulling = false;

	particle->setKinematicTarget(PxTransform(startPos));
}

void Plunger::Update(double t)
{
	if (pulling) {
		//Tiramos poco a poco hasta el tope
		pulledPos += PxVec3(0, 0, pullingVelocity * t);
		if (pulledPos.z > maxPulledPos)
			pulledPos.z = maxPulledPos;

		particle->setKinematicTarget(PxTransform(pulledPos));
	}
	else if (tweenTime > 0) {
		//Soltamos y se mete solo
		particle->setKinematicTarget(PxTransform(lerp(pulledPos, startPos, LerpValue())));

		tweenTime -= t;
	}
}
