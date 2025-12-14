#ifndef DOSM_FACTOR_HPP
#define DOSM_FACTOR_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"
#include "dosmLawLennardJones.hpp"
#include "dosmParallel.hpp"
#include <functional>
#include <vector>
#include <string>

namespace dosm
{
	class Factor
	{
		public:
			Factor(const std::string& file);
			~Factor(void) = default;

			void run(void); 

		private:
			void loadFile(const std::string& file);
			void outFile(void);

		private:
			std::vector<Particle> particles;
			DosmLawLennardJones dosmLawLJ;
			DosmParallel<std::function<void(idx_t)>> dosmParallel;
	};

} // namespace dosm

#endif // DOSM_FACTOR_HPP

