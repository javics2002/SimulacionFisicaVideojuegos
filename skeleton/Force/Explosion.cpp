#include "Explosion.h"
#include "../Particles/Particle.h"
#include <math.h>

Explosion::Explosion(PxVec3 center, double radius, double force, double duration)
	: center(center), radius(0), maxRadius(radius), force(force), duration(duration), time(0)
{
}

Explosion::~Explosion()
{
}

void Explosion::UpdateForce(Particle* p, double dt)
{
	time += dt;
	radius = maxRadius * time / duration;

	if (p != nullptr && p->GetInvMass() > DBL_EPSILON) {
		double distance = Distance(p, center);
		if((p->GetPos() - center).magnitudeSquared() < radius * radius)
			p->AddForce(force / pow(distance, 2) * (p->GetPos() - center) * exp(-time / duration));
	}
}

double Explosion::Distance(Particle* p, PxVec3 point)
{
	return sqrt(pow(p->GetPos().x - point.x, 2) 
		+ pow(p->GetPos().y - point.y, 2) 
		+ pow(p->GetPos().z - point.z, 2));
}