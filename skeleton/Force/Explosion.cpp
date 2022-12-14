#include "Explosion.h"
#include "../Particles/Particle.h"
#include <math.h>

Explosion::Explosion(PxVec3 center, double radius, double force, double duration)
	: center(center), radius(0), maxRadius(radius), force(force), duration(duration), time(0)
{
	type = EXPLOSION;
}

Explosion::~Explosion()
{
}

void Explosion::UpdateForce(Particle* p, double dt)
{
	time += dt;
	radius = maxRadius * time / duration;

	if (p != nullptr && p->GetInvMass() > DBL_EPSILON) {
		const double distanceSquared = (p->GetPos() - center).magnitudeSquared();
		if(distanceSquared < pow(radius, 2))
			p->AddForce(force / distanceSquared * (p->GetPos() - center) * exp(-time / duration));
	}
}