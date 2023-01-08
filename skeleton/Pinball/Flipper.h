#pragma once
#include "../Particles/RigidParticle.h"

#define degToRad(angleInDegrees) ((angleInDegrees) * PxPi / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / PxPi)

class Flipper : public RigidParticle
{
	bool left;

	//Position
	const float separation = .4f, speed = .08f;
	const PxVec3 rise = PxVec3(0, 0, -.14f);
	PxVec3 startPos = { -.1, 1.72, 1.5 };
	float lerp = 0;

public:
	Flipper(bool left);
	~Flipper();

	void Reset();

	void Update(double t);
};

