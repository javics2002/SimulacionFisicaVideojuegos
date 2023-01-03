#pragma once
#include "../Particles/RigidParticle.h"

class Ball : public RigidParticle
{
	const PxVec3 startPos = { 1.1, 1.6, 1.1 };

public:
	Ball();
	~Ball();

	void Reset();
};

