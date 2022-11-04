#pragma once
#include "Gravity.h"
#include "Drag.h"
#include "Wind.h"
#include "Whirlwind.h"
#include "Explosion.h"
#include <unordered_map>
#include <climits>

using namespace std;

class Particle;

class ForceRegistry : public unordered_multimap<ForceGenerator*, Particle*>
{
public:
	void Integrate(double dt);
	void AddRegistry(ForceGenerator* fg, Particle* p);
	void DeleteParticle(Particle* p);
	void DeleteForce(ForceGenerator* fg);
};

