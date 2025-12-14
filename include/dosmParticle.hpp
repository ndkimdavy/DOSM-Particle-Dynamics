#ifndef DOSM_PARTICLE_HPP
#define DOSM_PARTICLE_HPP

#include "dosmEntity.hpp"

namespace dosm
{
	class Particle : public Entity
	{
		public:
			r64_t energy;
			tensor_t<r64_t, 3> force;

			Particle(void) : Entity()
		{
			energy = 0.0;
			force(0) = force(1) = force(2) = 0.0;
		}

			explicit Particle(r64_t mass, r64_t charge = 0.0) : Entity(mass, charge) 
		{
			energy = 0.0;
			force(0) = force(1) = force(2) = 0.0;
		}

			virtual ~Particle(void) = default;
	};

} // namespace dosm

#endif // DOSM_PARTICLE_HPP

