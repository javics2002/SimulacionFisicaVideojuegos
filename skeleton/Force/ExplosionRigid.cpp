#include "ExplosionRigid.h"
#include "../Particles/RigidParticle.h"
#include "../Scene.h"

ExplosionRigid::ExplosionRigid(PxVec3 center, double radius, double force, double duration) 
	: center(center), radius(0), maxRadius(radius), force(force), 
	duration(duration), time(0)
{
	type = EXPLOSION_RIGID;
}

ExplosionRigid::~ExplosionRigid()
{
}

void ExplosionRigid::UpdateForce(RigidParticle* p, double dt)
{
	time += dt;
	if (time > duration) {
		active = false;
		return;
	}

	radius = maxRadius * time / duration;

	if (p != nullptr) {
		const double distanceSquared = (p->POS - center).magnitudeSquared();
		if (distanceSquared < pow(radius, 2))
			p->particle->addForce(force / distanceSquared * (p->POS - center) * 
				exp(-time / duration));
	}
}
