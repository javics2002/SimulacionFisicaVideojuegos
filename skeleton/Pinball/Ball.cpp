#include "Ball.h"

Ball::Ball() : RigidParticle({0, 0, 0}, CreateShape(PxSphereGeometry(0.01)), 
	{.8, .8, .8, 1}, METAL)
{
}

Ball::~Ball()
{
}

void Ball::Reset()
{
}
