#ifndef DOSM_FACTOR_HPP
#define DOSM_FACTOR_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"
#include <vector>
#include <string>

namespace dosm
{
	class Factor
	{
		public:
			Factor(const std::string& file);
			void run(void); 

		private:
			void loadFile(const std::string& xyz_file);

		private:
			std::vector<Particle> particles;
	};

} // namespace dosm

#endif // DOSM_FACTOR_HPP

