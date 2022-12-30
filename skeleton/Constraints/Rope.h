#pragma once
#include "Link.h"

class Rope : public Link
{
	float maxLength, restitution;

public:
	Rope(Particle* first, Particle* second, float length, float restitution);
	~Rope();

	void Update(double t) override;
};

