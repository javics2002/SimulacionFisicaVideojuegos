#include "Rope.h"
#include "../Particles/Particle.h"

unsigned Rope::AddContact(Contact* contact, unsigned limit) const
{
	//Find the length of the rope
	float length = CurrentLength();

	//Check if we're overextended
	if (length < maxLength)
		return 0;
	
	//Otherwise, return the contact

	//Calculate the normal
	contact->normal = (particle[1]->GetPos() - particle[0]->GetPos()).getNormalized();

	//Add information to contact
	contact->penetration;
	contact->restitution = restitution;

	return 1;
}
