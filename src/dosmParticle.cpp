#include "dosmParticle.hpp"

namespace dosm
{
	DosmParticle::DosmParticle(void) : DosmEntity()
	{
		energy = 0.0;
		force(0) = force(1) = force(2) = 0.0;
	}

	DosmParticle::DosmParticle(r64_t mass, r64_t charge) : DosmEntity(mass, charge)
	{
		energy = 0.0;
		force(0) = force(1) = force(2) = 0.0;
	}
}





