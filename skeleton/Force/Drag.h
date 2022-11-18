#pragma once
#include "ForceGenerator.h"

class Drag : public ForceGenerator
{
	float k1;
	float k2;

public:
	Drag(float k1 = 0.98, float k2 = 0);
	~Drag();
	void UpdateForce(Particle* p, double dt) override;
	inline void SetDrag(float k1_, float k2_);
	inline float GetK1();
	inline float GetK2();
};

