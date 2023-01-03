#include "Lerp.h"

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

PxVec3 lerp(PxVec3 a, PxVec3 b, float f) {
	return { lerp(a.x, b.x, f), lerp(a.y, b.y, f), lerp(a.z, b.z, f) };
}

PxVec4 lerp(PxVec4 a, PxVec4 b, float f) {
	return { lerp(a.x, b.x, f), lerp(a.y, b.y, f), lerp(a.z, b.z, f), lerp(a.w, b.w, f) };
}
