#include "dosmFactor.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

namespace dosm
{

	Factor::Factor(const std::string& file)
	{
		loadFile(file);
	}

	void Factor::loadFile(const std::string& file)
	{
		std::ifstream in(file);
		if (!in)
		{
			DOSM_LOG_ERROR("Cannot open file: " + file);
			throw std::runtime_error("dosmFactor: cannot open file");
		}

		i32_t h1, h2;
		in >> h1 >> h2;
		particles.clear();

		constexpr r64_t mass   = 1.0;
		constexpr r64_t charge = 0.0;

		i32_t type;
		r64_t x, y, z;
		while (in >> type >> x >> y >> z)
		{
			Particle p(mass, charge);
			p.position(0) = x;
			p.position(1) = y;
			p.position(2) = z;
			particles.push_back(p);
		}

		DOSM_LOG_INFO("Loaded " + std::to_string(particles.size()) + " particles");
	}

	void Factor::run(void)
	{
		DOSM_LOG_DEBUG("Factor::run() entered");
	}

} // namespace dosm

