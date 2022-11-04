#pragma once
#include "ForceGenerator.h"

class Gravity : public ForceGenerator
{
    PxVec3 g;
public:
    Gravity(PxVec3 gravity = PxVec3(0, -9.8, 0));
    ~Gravity();
    void UpdateForce(Particle* p, double dt) override;
};

