#include "Contact.h"
#include "../Particles/Particle.h"

void Contact::ResolveVelocity(float t)
{
	//Compute impulse for each particle
	PxVec3 impulse = -(particle[0]->GetVel() - particle[1]->GetVel()) * (restitution + 1) /
		(particle[0]->GetInvMass() * particle[1]->GetInvMass());

	//Set new velocity for each particle
	for(int i = 0; i < 2; i++)
		particle[i]->SetVel(particle[i]->GetVel() + particle[i]->GetInvMass() * impulse);
}

void Contact::ResolveInterpenetration(float t)
{
	//Check if there is interpenetration
	PxVec3 penetrationNormal = (particle[1]->GetPos() + particle[1]->GetRadius() * -normal -
		particle[0]->GetPos() + particle[0]->GetRadius() * normal).getNormalized();
	if (penetrationNormal != -normal)
		return;

	//Calculate factor of ecah particle depending on mass
	float factor[2] = { particle[1]->GetMass() / (particle[0]->GetMass() + particle[1]->GetMass()),
		particle[0]->GetMass() / (particle[0]->GetMass() + particle[1]->GetMass()) };

	//Calculate the movement for each particle
	PxVec3 movement[2] = { factor[0] * normal, factor[1] * normal };

	//Apply movement to the position of each particle
	for (int i = 0; i < 2; i++)
		particle[i]->Translate(movement[i]);
}

void Contact::Resolve(float t)
{
	ResolveVelocity(t);
	ResolveInterpenetration(t);
}
