#ifndef DOSM_LAW_LENNARD_JONES_PERIODIC_HPP
#define DOSM_LAW_LENNARD_JONES_PERIODIC_HPP

#include "dosmLawLennardJones.hpp"

namespace dosm
{
	class DosmLawLennardJonesPeriodic : public DosmLawLennardJones
	{
		public:
			DosmLawLennardJonesPeriodic(vector_t<DosmParticle>& particles, r64_t sigma, r64_t epsilon, r64_t boxLength, r64_t rayCut);
			~DosmLawLennardJonesPeriodic(void) = default;
			void kernel(Result* result) override;

		protected:
			r64_t boxLength;
			r64_t rayCut;
			vector_t<tensor_t<r64_t, 3>> images;
	};

} // namespace dosm

#endif // DOSM_LAW_LENNARD_JONES_PERIODIC_HPP

