#include "AnchoredRealSpring.h"
#include "../Particles/Particle.h"
#include "../RenderUtils.hpp"

AnchoredRealSpring::AnchoredRealSpring(const PxVec3& anchorPos, double k, double restLength,
	double minLength, double maxLength, double breakingLength)
	: RealSpring((new Particle(anchorPos))->SetIMass(1e6)->SetColor({ 0.67, 0.36, 0.12, 1 })
		->SetShape(CreateShape(PxBoxGeometry(.2, .2, .2))), k, restLength,
		minLength, maxLength, breakingLength)
{
}

AnchoredRealSpring::~AnchoredRealSpring()
{
	delete other;
}
