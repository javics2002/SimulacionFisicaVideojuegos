#pragma once
#include <unordered_map>
#include "Gravity.h"
#include "Drag.h"
#include "Wind.h"
#include "Whirlwind.h"
#include "Explosion.h"

using namespace std;

class Particle;

class ForceRegistry : unordered_multimap<ForceGenerator*, Particle*>
{
public:
	void Integrate(double dt);
	void AddRegistry(ForceGenerator* fg, Particle* p);
	void DeleteParticle(Particle* p);
};

