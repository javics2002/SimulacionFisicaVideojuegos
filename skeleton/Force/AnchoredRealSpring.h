#pragma once
#include "RealSpring.h"

class AnchoredRealSpring : public RealSpring
{
public:
	AnchoredRealSpring(const PxVec3& anchorPos, double k, double restLength,
		double minLength, double maxLength, double breakingLength);
	~AnchoredRealSpring() override;
};

