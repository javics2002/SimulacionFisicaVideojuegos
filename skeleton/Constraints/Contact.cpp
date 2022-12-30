#include "Contact.h"
#include "../Particles/Particle.h"

void Contact::ResolveVelocity(double t)
{
	//El momento se reparte
	PxVec3 momentum = (particle[0]->GetVel() * particle[0]->GetMass()
		+ particle[1]->GetVel() * particle[1]->GetMass()) * .5;

	for (int i = 0; i < 2; i++) 
		particle[i]->SetVel(momentum * particle[i]->GetInvMass());
}

void Contact::ResolveInterpenetration(double t)
{
	//Calculate factor of ecah particle depending on mass
	float factor[2] = { particle[0]->GetInvMass() * particle[1]->GetInvMass() / particle[0]->GetInvMass(),
		particle[0]->GetInvMass()* particle[1]->GetInvMass() / particle[1]->GetInvMass() };

	//Calculate the movement for each particle
	PxVec3 v = particle[1]->GetPos() - particle[0]->GetPos();
	float distance = v.normalize();
	float underLength = radius[0] + radius[1] - distance;
	PxVec3 movement[2] = { -factor[0] * underLength * v, factor[1] * underLength * v };

	//Apply movement to the position of each particle
	for (int i = 0; i < 2; i++)
		particle[i]->Translate(movement[i]);
}

Contact::Contact(Particle* first, float radius1, Particle* second, float radius2, float restitution)
	: restitution(restitution)
{
	particle[0] = first;
	radius[0] = radius1;
	particle[1] = second;
	radius[1] = radius2;
}

Contact::~Contact()
{
}

void Contact::Update(double t)
{
	//Check if there is interpenetration
	//Si sus centros estan mas cercanos que la suma de sus radios
	if ((particle[1]->GetPos() - particle[0]->GetPos()).magnitudeSquared()
		< pow(radius[0] + radius[1], 2))
		Resolve(t);
}

void Contact::Resolve(double t)
{
	ResolveVelocity(t);
	ResolveInterpenetration(t);
}
