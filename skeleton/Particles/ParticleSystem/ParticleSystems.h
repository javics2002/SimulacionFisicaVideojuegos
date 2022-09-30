#pragma once
#include "ParticleSystem.h"
#include "SimpleParticleGenerator.h"
#include "GaussianParticleGenerator.h"

enum ParticleSystemType { FOUNTAIN, MIST, DUST, FIRE, RAIN, SPARK, BLAST };

static ParticleGenerator* CreateGenerator(ParticleSystemType type) {
	switch (type) {
	case FOUNTAIN:
		return;
	case MIST:
		return;
	case DUST:
		return;
	case FIRE:
		return;
	case RAIN:
		return;
	case SPARK:
		return;
	case BLAST:
		return;
	default:
		return new SimpleParticleGenerator(new Particle());
	}
}