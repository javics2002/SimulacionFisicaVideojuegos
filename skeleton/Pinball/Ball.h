#pragma once
#include "../Particles/RigidParticle.h"

class Ball : public RigidParticle
{
	const PxVec3 startPos = {.3, 1.72, 0};//{ 1.1, 1.72, 1.1 };

public:
	Ball();
	~Ball();

	void Reset();
};

