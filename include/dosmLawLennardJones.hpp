#ifndef DOSM_LAW_LENNARD_JONES_HPP
#define DOSM_LAW_LENNARD_JONES_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"
#include <vector>

namespace dosm
{
	class DosmLawLennardJones
	{
		public:
			struct Result
			{
				r64_t energy;    
				std::vector<Particle>* particles; 
			};

			DosmLawLennardJones(void) = default;
			DosmLawLennardJones(std::vector<Particle> particles, r64_t sigma = 1.0, r64_t epsilon = 1.0);
			~DosmLawLennardJones(void) = default;
			
			void kernel(Result* result);

		private:
			r64_t sigma;
			r64_t epsilon;
			std::vector<Particle> particles; 
	};

} // namespace dosm

#endif // DOSM_LAW_LENNARD_JONES_HPP

