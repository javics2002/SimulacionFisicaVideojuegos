#include "Flipper.h"

double Flipper::LerpValue()
{
	if (2 * tweenTime / flipTime < 1)
		return 2 * tweenTime / flipTime;
	else
		return 2 - 2 * tweenTime / flipTime;
}

Flipper::Flipper(bool left) : RigidParticle(startPos, .1, CreateShape(PxBoxGeometry(PxVec3(.225, .036, .05))),
	{ 1, 1, 0, 1 }, METAL), left(left)
{
	//Cada pala va a un lado
	startPos.x = pow(-1, left) * separation;

	//Las palas las quiero controlar yo. Las marco como cinematicas
	particle->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	tweenTime = 0;
}

Flipper::~Flipper()
{
}

void Flipper::Flip()
{
	if(tweenTime <= 0)
		tweenTime = flipTime;
}

void Flipper::Reset()
{
	particle->setKinematicTarget(PxTransform(startPos, 
		PxQuat(degToRad(pow(-1, left) * 20), PxVec3(0, 1, 0))));
}

void Flipper::Update(double t)
{
	if (tweenTime > 0) {
		double l = LerpValue();

		particle->setKinematicTarget(PxTransform(startPos + l * rise,
			PxQuat(degToRad(pow(-1, left) * -20 * (2 * l - 1)), PxVec3(0, 1, 0))));

		tweenTime -= t;
	}
}
