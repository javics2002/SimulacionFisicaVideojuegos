#include "Ball.h"

Ball::Ball() : RigidParticle(startPos, .04, CreateShape(PxSphereGeometry(0.02)),
	{.8, .8, .8, 1}, SOAP)
{
}

Ball::~Ball()
{
}

void Ball::Reset()
{
	particle->setGlobalPose(PxTransform(startPos));
	particle->setLinearVelocity(PxVec3(0));
}
