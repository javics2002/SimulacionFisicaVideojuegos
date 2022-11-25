#include "AnchoredSpring.h"
#include "../Particles/Particle.h"
#include "../RenderUtils.hpp"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

AnchoredSpring::AnchoredSpring(const PxVec3& anchorPos, double k, double restLength)
	: Spring((DBG_NEW Particle(anchorPos))->SetIMass(1e6)->SetShape(CreateShape(PxBoxGeometry(.1, .1, .1))), k, restLength)
{
}

AnchoredSpring::~AnchoredSpring()
{
	delete other;
}
