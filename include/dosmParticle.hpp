#ifndef DOSM_PARTICLE_HPP
#define DOSM_PARTICLE_HPP

#include "dosmEntity.hpp"

namespace dosm
{
	class DosmParticle : public DosmEntity
	{
		public:
			r64_t p_energy;
			r64_t k_energy;
			tensor_t<r64_t, 3> momentum;
			tensor_t<r64_t, 3> force;
			
			DosmParticle(void);
			explicit DosmParticle(r64_t mass, r64_t charge);
			virtual ~DosmParticle(void) = default;
	};

	struct DosmParticleSnap
	{
		struct Snap
		{
			r64_t t;
			vector_t<DosmParticle> particles;
		};

		vector_t<Snap> snaps;
	};
}

#endif // DOSM_PARTICLE_HPP
