#include "dosmEntity.hpp"

namespace dosm
{
	DosmEntity::DosmEntity(void)
	{
		mass = 0.0;
		charge = 0.0;
		position(0) = position(1) = position(2) = 0.0;
		velocity(0) = velocity(1) = velocity(2) = 0.0;
	}

	DosmEntity::DosmEntity(r64_t mass, r64_t charge)
	{
		this->mass = mass;
		this->charge = charge;
		position(0) = position(1) = position(2) = 0.0;
		velocity(0) = velocity(1) = velocity(2) = 0.0;
	}
}

