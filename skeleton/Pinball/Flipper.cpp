#include "Flipper.h"

Flipper::Flipper() : RigidParticle({ 0, 0, 0 }, CreateShape(PxBoxGeometry(PxVec3(1, 5, .8))),
	{ 1, 0, 1, 1 }, METAL)
{
}

Flipper::~Flipper()
{
}

void Flipper::Reset()
{
}
