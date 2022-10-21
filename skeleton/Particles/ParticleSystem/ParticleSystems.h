#pragma once
#include "ParticleSystem.h"
#include "SimpleParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "BoxParticleGenerator.h"

enum ParticleSystemType { FOUNTAIN, MIST, DUST, FIRE, RAIN, SPARK, BLAST, SNOW };

static ParticleGenerator* CreateGenerator(ParticleSystemType type) {
	Particle* prefab = new Particle({ 0, 0, 0 }, false);

	switch (type) {
	case FOUNTAIN:
		prefab->SetVel({ 0, 30, 0 })->SetDamp(.8)->SetAcc({ -.2, -10, 0 })
			->SetColor({ .3, .8, .95, .9 })->SetShape(CreateShape(PxSphereGeometry(.1)));
		return new GaussianParticleGenerator(prefab, 0, .5);
	case MIST:
		prefab->SetVel({.2, 0, .15})->SetColor({ .9, .9, .9, .1 })
			->SetShape(CreateShape(PxSphereGeometry(.01)));
		return new BoxParticleGenerator(prefab, 10000, { 40, 45, 40 }, { 1, 1, 1 }, { .1, .1, .1 });
	case DUST:
		prefab->SetShape(CreateShape(PxSphereGeometry(.05)));
		return new BoxParticleGenerator(prefab, 10000, { 0, 0, 0 }, { 100, 100, 100 }, { .1,.1,.1 });
	case FIRE:
		//return;
	case RAIN:
		//return;
	case SPARK:
		//return;
	case BLAST:
		//return;
	case SNOW:
		prefab->SetAcc({ .2, -2, .5 })->SetShape(CreateShape(PxSphereGeometry(.05)));
		return new BoxParticleGenerator(prefab, 10000, { 0, 0, 0 }, { 100, 100, 100 }, { .1,.1,.1 });
	default:
		return new SimpleParticleGenerator(prefab);
	}
}