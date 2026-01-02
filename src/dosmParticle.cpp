#include "dosmParticle.hpp"

namespace dosm
{
	DosmParticle::DosmParticle(void) : DosmEntity()
	{
		p_energy = 0.0;
		k_energy = 0.0;
		momentum(0) = momentum(1) = momentum(2) = 0.0;
		force(0) = force(1) = force(2) = 0.0;
	}

	DosmParticle::DosmParticle(r64_t mass, r64_t charge) : DosmEntity(mass, charge)
	{
		p_energy = 0.0;
		k_energy = 0.0;
		momentum(0) = momentum(1) = momentum(2) = 0.0;
		force(0) = force(1) = force(2) = 0.0;
	}
}





