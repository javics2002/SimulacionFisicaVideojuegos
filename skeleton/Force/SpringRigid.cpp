#include "SpringRigid.h"
#include "../Particles/RigidParticle.h"

SpringRigid::SpringRigid(RigidParticle* other, double k, double restLength)
{
    type = SPRING_RIGID;
}

SpringRigid::~SpringRigid()
{
}

void SpringRigid::UpdateForce(RigidParticle* p, double dt)
{
    PxVec3 force = other->POS - p->POS;
    p->particle->addForce(force * (force.magnitude() - restLength) * k);
}

double SpringRigid::AddK(double increment)
{
    k += increment;

    if (k < 1)
        k = 1;

    return k;
}

double SpringRigid::AddRestLength(double increment)
{
    restLength += increment;

    if (restLength < DBL_EPSILON)
        restLength = DBL_EPSILON;

    return restLength;
}
