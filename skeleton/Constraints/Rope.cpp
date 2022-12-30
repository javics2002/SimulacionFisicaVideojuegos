#include "Rope.h"
#include "../Particles/Particle.h"

Rope::Rope(Particle* first, Particle* second, float length, float restitution)
	: Link(first, second), maxLength(length), restitution(restitution)
{
}

Rope::~Rope()
{
}

void Rope::Update(double t)
{
	PxVec3 v = particle[1]->GetPos() - particle[0]->GetPos();
	float distance = v.normalize();
	float nextDistanceSquared = (particle[1]->GetPos() + particle[1]->GetVel() 
		- particle[0]->GetPos() - particle[0]->GetVel()).magnitudeSquared();

	//Check if we're overextended
	if (distance < maxLength)
		return;

	//Si ya se estan acercando, no voy a interferir
	if (pow(distance, 2) > nextDistanceSquared)
		return;

	//El momento se reparte
	PxVec3 momentum = (particle[0]->GetVel() * particle[0]->GetMass()
		+ particle[1]->GetVel() * particle[1]->GetMass()) * .5;

	for (int i = 0; i < 2; i++)
		particle[i]->SetVel(momentum * particle[i]->GetInvMass());

	//Restrinjimos la distancia de las particilas
	float overLength = distance - maxLength;
	particle[0]->SetPos(particle[0]->GetPos() + v
		* overLength * particle[0]->GetInvMass() * particle[1]->GetInvMass() / particle[0]->GetInvMass());
	particle[1]->SetPos(particle[1]->GetPos() - v
		* overLength * particle[0]->GetInvMass() * particle[1]->GetInvMass() / particle[1]->GetInvMass());
}
