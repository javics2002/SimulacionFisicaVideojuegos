#pragma once
#include "../Particles/RigidParticle.h"

#define degToRad(angleInDegrees) ((angleInDegrees) * PxPi / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / PxPi)

class Flipper : public RigidParticle
{
	bool left;

	//Position
	const float separation = .4f;
	const PxVec3 rise = PxVec3(0, 0, -.1f);
	PxVec3 startPos = { 0, 1.75, 1.5 };

	//Timing
	const double flipTime = .2; //Cantidad de tiempo que una pala tarda en subir y bajar tras pulsarla
	double tweenTime; //Valor del tween que va moviendo la pala desde flipTime hasta 0

	double LerpValue();

public:
	Flipper(bool left);
	~Flipper();

	void Flip();
	void Reset();

	void Update(double t);
};

