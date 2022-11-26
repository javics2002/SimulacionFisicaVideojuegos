#include "Drag.h"
#include "../Particles/Particle.h"

Drag::Drag(float k1, float k2) : k1(k1), k2(k2)
{
	type = DRAG;
}

Drag::~Drag()
{
}

void Drag::UpdateForce(Particle* p, double dt)
{
	if (p != nullptr && p->GetInvMass() > DBL_EPSILON) {
		float dragCoef = p->GetVel().magnitude();
		dragCoef = k1 * dragCoef + k1 * dragCoef * dragCoef;
		p->AddForce(-p->GetVel().getNormalized() * dragCoef);
	}
}

inline void Drag::SetDrag(float k1_, float k2_)
{
	k1 = k1_;
	k2 = k2_;
}

inline float Drag::GetK1()
{
	return k1;
}

inline float Drag::GetK2()
{
	return k1;
}
