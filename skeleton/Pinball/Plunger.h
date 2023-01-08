#pragma once
#include "../Particles/RigidParticle.h"

class Plunger : public RigidParticle
{
	bool pulling = false;

	//Position
	const PxVec3 startPos = { 1.1, 1.72, 1.5 };
	PxVec3 pulledPos;
	const float maxPulledPos = 1.8;

	//Timing
	const double pullingVelocity = .1;
	double plungeTime = .07; //Cantidad de tiempo que tarda el piston en meterse
	double tweenTime; //Valor del tween que va moviendo la pala desde plungeTime hasta 0

	double LerpValue();

public:
	Plunger();
	~Plunger();

	void Plunge();
	void Reset();

	void Update(double t);
};

