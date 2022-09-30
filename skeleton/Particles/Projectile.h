#pragma once

#include <cmath>

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

