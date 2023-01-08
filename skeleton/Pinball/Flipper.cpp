#include "Flipper.h"

Flipper::Flipper(bool left) : RigidParticle(startPos, .1, 
	CreateShape(PxBoxGeometry(PxVec3(.225, .04, .05))), { 1, 1, 0, 1 }, FLIPPER), left(left)
{
	//Cada pala va a un lado
	startPos.x += pow(-1, left) * separation;

	//Las palas las quiero controlar yo. Las marco como cinematicas
	particle->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

Flipper::~Flipper()
{
}

void Flipper::Reset()
{
	particle->setKinematicTarget(PxTransform(startPos, 
		PxQuat(degToRad(pow(-1, left) * 20), PxVec3(0, 1, 0))));
}

void Flipper::Update(double t)
{
	lerp += GetAsyncKeyState(left ? 'C' : 'N') ? speed : -speed;
	lerp = PxClamp<float>(lerp, 0, 1);

	particle->setKinematicTarget(PxTransform(startPos + lerp * rise,
		PxQuat(degToRad(pow(-1, left) * -20 * (2 * lerp - 1)), PxVec3(0, 1, 0))));
}
