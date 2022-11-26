#pragma once
#include "Gravity.h"
#include "Drag.h"
#include "Wind.h"
#include "Whirlwind.h"
#include "Explosion.h"
#include "AnchoredSpring.h"
#include "Impulse.h"
#include "RubberBand.h"
#include <unordered_map>
#include <climits>

using namespace std;

class Particle;
typedef unordered_multimap<ForceGenerator*, Particle*> mapFR;

class ForceRegistry : public mapFR
{
public:
	void Integrate(double dt);
	mapFR::iterator AddRegistry(ForceGenerator* fg, Particle* p);
	void DeleteParticle(Particle* p);
	void DeleteForce(ForceGenerator* fg);
};

