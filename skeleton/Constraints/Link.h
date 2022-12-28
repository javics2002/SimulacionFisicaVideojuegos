#pragma once
#include "Contact.h"

class Link : public Contact
{
protected:
	float CurrentLength() const;

public:
	Particle* particle[2];
	virtual unsigned AddContact(Contact* contact, unsigned limit) const = 0;
};

