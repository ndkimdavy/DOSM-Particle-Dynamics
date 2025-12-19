#ifndef DOSM_LAW_LENNARD_JONES_HPP
#define DOSM_LAW_LENNARD_JONES_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"

namespace dosm
{
	class DosmLawLennardJones
	{
		public:
			struct Result
			{
				r64_t energy;    
				vector_t<DosmParticle>* particles; 
			};

			DosmLawLennardJones(void) = default;
			DosmLawLennardJones(vector_t<DosmParticle> particles, r64_t sigma, r64_t epsilon);
			~DosmLawLennardJones(void) = default;

			void kernel(Result* result);

		private:
			r64_t sigma;
			r64_t epsilon;
			vector_t<DosmParticle> particles; 
	};

} // namespace dosm

#endif // DOSM_LAW_LENNARD_JONES_HPP

