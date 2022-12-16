#include "RealSpring.h"

RealSpring::RealSpring(Particle* other, double k, double restLength,
	double minLength, double maxLength, double breakingLength)
	: Spring(other, k, restLength), minLength(minLength), 
	maxLength(maxLength), breakingLength(breakingLength)
{
}

RealSpring::~RealSpring()
{
}
