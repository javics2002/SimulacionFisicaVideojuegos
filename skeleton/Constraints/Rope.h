#pragma once
#include "Link.h"

class Rope : public Link
{
public:
	float maxLength, restitution;
	virtual unsigned AddContact(Contact* contact, unsigned limit) const;
};

