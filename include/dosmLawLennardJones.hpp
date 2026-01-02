#ifndef DOSM_LAW_LENNARD_JONES_HPP
#define DOSM_LAW_LENNARD_JONES_HPP

#include "idosmLaw.hpp"

namespace dosm
{
	class DosmLawLennardJones : public IDosmLaw
	{
		public:
			DosmLawLennardJones(vector_t<DosmParticle>& particles, r64_t sigma, r64_t epsilon);
			~DosmLawLennardJones(void) = default;
			void kernel(Result* result) override;

		protected:
			vector_t<DosmParticle>& particles;
			r64_t sigma;
			r64_t epsilon;
	};

} // namespace dosm

#endif // DOSM_LAW_LENNARD_JONES_HPP

