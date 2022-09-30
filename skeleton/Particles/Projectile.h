#pragma once

#include "Particles/Particle.h"
#include "RenderUtils.hpp"
#include <cmath>
#include <cfloat>

const double g = -9.8;
const double c = 299792458;

enum ProjectileType { PISTOL, ARTILLERY, FIREBALL, LASER, GOLF, FRISBEE, BALLOON };

struct Projectile {
	double inverseMass; //inversa de la masa
	double speed; //velocidad
	double gravity; //gravedad
	double initialHeight; //altura inicial
	double angle;
	double variation;
};

static Projectile Simulate(double simulatedVel, Projectile real) {
	Projectile simulated;
	simulated.speed = simulatedVel;
	simulated.initialHeight = real.initialHeight;
	simulated.angle = real.angle;
	simulated.variation = real.variation;

	//Conserva la energia	E = mv
	if (real.speed == 0)
		simulated.inverseMass = real.inverseMass;
	else
		simulated.inverseMass = real.inverseMass * pow(simulated.speed / real.speed, 2);

	//Queremos que recorra la misma distancia	0 = p + vt + .5 * at^2
	if (simulatedVel == 0)
		simulated.gravity = real.gravity;
	else if (real.gravity != 0) {
		const double realFlightTime = sqrt(2 * real.initialHeight / abs(real.gravity));
		const double distanceReached = real.speed * realFlightTime;
		const double simulatedFlightTime = distanceReached / simulated.speed;

		simulated.gravity = real.gravity * pow(realFlightTime, 2) / pow(simulatedFlightTime, 2);
	}
	else
		simulated.gravity = 0;

	return simulated;
}

static Projectile CreateProjectile(ProjectileType type, const physx::PxTransform& camera, Particle* p) {
	Projectile real, simulated;
	real.initialHeight = camera.p.y;
	real.angle = 0;

	switch (type) {
	case PISTOL:
		p->SetShape(CreateShape(PxSphereGeometry(0.1)));
		p->SetColor({ .7, .7, .7, 1 });

		//Masa real: 0.18 kg	Velocidad real: 360 m/s
		real.inverseMass = 1 / .18;
		real.speed = 360;
		real.gravity = g;
		real.variation = .1;
		p->SetDamp(.95);

		//Velocidad deseada: 40 m/s
		simulated.speed = 40;
		break;
	case ARTILLERY:
		p->SetShape(CreateShape(PxSphereGeometry(.5)));
		p->SetColor({ .2, .2, .2, 1 });

		//Masa real: 50 kg	Velocidad real: 1800 m/s
		real.inverseMass = 1 / 50.0;
		real.speed = 1800;
		real.gravity = g;
		real.variation = .2;
		p->SetDamp(.9);

		//Velocidad deseada: 20 m/s
		simulated.speed = 20;
		break;
	case FIREBALL:
		p->SetShape(CreateShape(PxSphereGeometry(0.2)));
		p->SetColor({ 1, .4, .1, 1 });

		//Masa real: 2 kg	Velocidad real: 10 m/s
		real.inverseMass = 1 / 2.0;
		real.speed = 10;
		real.gravity = 1;
		real.variation = .4;

		//Velocidad deseada: 10 m/s
		simulated.speed = 10;
		break;
	case LASER:
		p->SetShape(CreateShape(PxSphereGeometry(0.1)));
		p->SetColor({ 1, .3, .3, 1 });

		//Masa real: 0 kg	Velocidad real: c m/s
		real.inverseMass = DBL_MAX;
		real.speed = c;
		real.gravity = 0;
		real.variation = 0;

		//Velocidad deseada: 50 m/s
		simulated.speed = 50;
		break;
	case GOLF:
		p->SetShape(CreateShape(PxSphereGeometry(0.13)));
		p->SetColor({ .9, .9, .9, 1 });

		//Masa real: 0.045 kg	Velocidad real: 30 m/s (A media distancia)
		real.inverseMass = 1 / .045;
		real.speed = 30;
		real.gravity = g;
		real.variation = 2;
		real.angle = PxPi / 6;
		p->SetDamp(0.8);

		//Velocidad deseada: 20 m/s
		simulated.speed = 20;
		break;
	case FRISBEE:
		p->SetShape(CreateShape(PxBoxGeometry(.2, .01, .2)));
		p->SetColor({ .7, .01, 0, 1 });

		//Masa real: .175 kg	Velocidad real: 10 m/s
		real.inverseMass = 1 / .175;
		real.speed = 10;
		real.gravity = g;
		real.variation = 1;
		real.angle = PxPi / 12;
		p->SetDamp(0.7);

		//Velocidad deseada: 50 m/s
		simulated.speed = 5;
		break;
	case BALLOON:
		p->SetShape(CreateShape(PxSphereGeometry(.3)));
		p->SetColor({ 1, .3, .3, 1 });

		//Masa real: 0.001 kg	Velocidad real: 0 m/s
		real.inverseMass = 1 / .001;
		real.speed = 0;
		real.gravity = -.1 * g;
		real.variation = 0.1;
		p->SetDamp(.3);

		//Velocidad deseada: 0 m/s
		simulated.speed = 0;
		break;
	default:
		break;
	}

	return Simulate(simulated.speed, real);
}