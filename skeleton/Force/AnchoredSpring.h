#pragma once
#include "Spring.h"

class AnchoredSpring : public Spring
{
public:
	AnchoredSpring(const PxVec3& anchorPos, double k, double restLength);
	~AnchoredSpring() override;
};

