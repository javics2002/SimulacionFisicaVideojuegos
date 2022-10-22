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
		return new BoxParticleGenerator(prefab, 10000, { 48, 49, 48 }, { 1, 1, 1 }, { .1, .1, .1 });
	case DUST:
		prefab->SetShape(CreateShape(PxSphereGeometry(.05)));
		return new BoxParticleGenerator(prefab, 10000, { 0, 0, 0 }, { 100, 100, 100 }, { .1,.1,.1 });
	case FIRE:
		prefab->SetVel({ 0, 0, 0 })->SetDamp(.8)->SetAcc({ -.2, 1, .1 })
			->SetColor({ 1, .7, 0, 1 })->SetEndColor({ .6, 0, 0, 1 })
			->SetShape(CreateShape(PxSphereGeometry(.2)))->SetLifetime(5);
		return new GaussianParticleGenerator(prefab, 0, 1);
	case RAIN:
		prefab->SetVel({ 0, -10, .5 })->SetAcc({ .2, -10, .5 })->SetDamp(.9)->SetShape(CreateShape(PxSphereGeometry(.02)))
			->SetColor({.4f, .5f, .9f, 1});
		return new BoxParticleGenerator(prefab, 10000, { 0, 100, 0 }, { 50, 100, 50 }, { .1,.1,.1 });
	case SPARK:
		prefab->SetVel({ 0, 10, 0 })->SetAcc({ 0, -10, 0 })
			->SetColor({ 1, 1, 1, 1 })->SetEndColor({ 1, 1, 0, 1 })
			->SetShape(CreateShape(PxSphereGeometry(.2)))->SetLifetime(1);
		return new GaussianParticleGenerator(prefab, 5, 4);
	case BLAST:
		prefab->SetVel({ 0, 10, 0 })->SetAcc({ 0, -10, 0 })
			->SetColor({ 1, 1, .9, 1 })->SetEndColor({ .5, .3, 0, 1 })
			->SetShape(CreateShape(PxSphereGeometry(1)))->SetLifetime(3);
		return new BoxParticleGenerator(prefab, 5000, { 0, 10, 0 }, { .5, .5, .5 }, { 20, 20, 20 });
	case SNOW:
		prefab->SetAcc({ .2, -2, .5 })->SetDamp(.3)->SetShape(CreateShape(PxSphereGeometry(.05)));
		return new BoxParticleGenerator(prefab, 10000, { 0, 100, 0 }, { 100, 100, 100 }, { .1,.1,.1 });
	default:
		return new SimpleParticleGenerator(prefab);
	}
}