#pragma once
#include "ParticleSystem.h"
#include "SimpleParticleGenerator.h"
#include "GaussianParticleGenerator.h"

enum ParticleSystemType { FOUNTAIN, MIST, DUST, FIRE, RAIN, SPARK, BLAST };

static ParticleGenerator* CreateGenerator(ParticleSystemType type) {
	Particle* prefab = new Particle({0, 0, 0}, false);

	switch (type) {
	case FOUNTAIN:
		prefab->SetVel({ 0, 30, 0 })->SetDamp(.8)->SetAcc({ -.2, -10, 0 })
			->SetColor({ .3, .8, .95, .9 })->SetShape(CreateShape(PxSphereGeometry(.1)));
		return new GaussianParticleGenerator(prefab, 0, .5);
	case MIST:
		//return;
	case DUST:
		//return;
	case FIRE:
		//return;
	case RAIN:
		//return;
	case SPARK:
		//return;
	case BLAST:
		//return;
	default:
		return new SimpleParticleGenerator(prefab);
	}
}