#include "Buoyancy.h"
#include "../Particles/Particle.h"
#include "../RenderUtils.hpp"

Buoyancy::Buoyancy(PxVec3 surfacePos, float height, float density, float volume, float gravity) :
	height(height), density(density), volume(volume), gravity(gravity)
{
	liquidSurface = (new Particle(surfacePos))->SetShape(CreateShape(PxBoxGeometry(1, .01, 1)))
		->SetColor({ 0, 0, 1, .3 });
}

Buoyancy::~Buoyancy()
{
	delete liquidSurface;
}

void Buoyancy::UpdateForce(Particle* p, double dt)
{
	float h = p->GetPos().y;
	float h0 = liquidSurface->GetPos().y;

	float immersed;
	if (h - h0 > height * .5)
		immersed = 0;
	else if (h - h0 > height * .5)
		immersed = 1;
	else
		immersed = (h0 - h) / height + .5;

	p->AddForce({0, density * volume * immersed * gravity, 0});
}
