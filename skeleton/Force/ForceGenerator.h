#pragma once

using namespace std;

class Particle;

class ForceGenerator
{
public:
	virtual void UpdateForce(Particle* p, double dt) = 0;
};

