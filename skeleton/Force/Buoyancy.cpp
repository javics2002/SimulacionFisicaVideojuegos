#include "Buoyancy.h"
#include "../Particles/Particle.h"
#include "../RenderUtils.hpp"
#include "Drag.h"

Buoyancy::Buoyancy(PxVec3 surfacePos, float height, float density, float volume, float gravity) :
	height(height), density(density), volume(volume), gravity(gravity)
{
	type = BUOYANCY;

	liquidSurface = (new Particle(surfacePos))->SetShape(CreateShape(PxBoxGeometry(1, .01, 1)))
		->SetColor({ 0, 0, 1, .3 });

	liquidDrag = new Drag(.1);
	gasDrag = new Drag(0, .1);
}

Buoyancy::~Buoyancy()
{
	delete liquidSurface;
	delete liquidDrag;
	delete gasDrag;
}

void Buoyancy::UpdateForce(Particle* p, double dt)
{
	float h = p->GetPos().y;
	float h0 = liquidSurface->GetPos().y;

	float immersed;
	if (h - h0 > height * .5) {
		immersed = 0;
		gasDrag->UpdateForce(p, dt);
	}
	else if (h0 - h > height * .5) {
		immersed = 1;
		liquidDrag->UpdateForce(p, dt);
	}
	else
		immersed = (h0 - h) / height + .5;

	p->AddForce({0, density * volume * immersed * gravity, 0});
}

float Buoyancy::AddDensity(float increment)
{
	density += increment;
	if (density < .1)
		density = .1;
	return density;
}

Buoyancy* Buoyancy::SetHeight(float newHeight)
{
	height = newHeight;
	return this;
}

Buoyancy* Buoyancy::SetVolume(float newVolume)
{
	volume = newVolume;
	return this;
}
