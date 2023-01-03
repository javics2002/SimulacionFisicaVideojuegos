#pragma once
#include <PxPhysicsAPI.h>

using namespace physx;

float lerp(float a, float b, float f);
PxVec3 lerp(PxVec3 a, PxVec3 b, float f);
PxVec4 lerp(PxVec4 a, PxVec4 b, float f);
