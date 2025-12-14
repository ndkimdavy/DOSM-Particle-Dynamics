#include "dosmFactor.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

namespace dosm
{

	Factor::Factor(const std::string& file)
	{
		loadFile(file);
		dosmLawLJ = DosmLawLennardJones(particles, 1.0, 1.0);
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
		ui64_t count = 0;
		while (in >> type >> x >> y >> z)
		{
			Particle p(mass, charge);
			p.position(0) = x;
			p.position(1) = y;
			p.position(2) = z;
			particles.push_back(p);
			++count;
		}

		DOSM_LOG_INFO("Loaded " + std::to_string(particles.size()) + " particles");
	}

	void Factor::outFile(void)
	{
		std::ofstream out("output.txt");
		if (!out)
		{
			DOSM_LOG_ERROR("Cannot open file: output.txt");
			throw std::runtime_error("dosmFactor: cannot open file");
		}

		out << "# i  x  y  z   fx  fy  fz   energy\n";
		idx_t n = particles.size();
		for (idx_t i = 0; i < n; ++i)
		{
			const Particle& p = particles[i];
			out << i << " "
				<< p.position(0) << " " << p.position(1) << " " << p.position(2) << " "
				<< p.force(0)    << " " << p.force(1)    << " " << p.force(2)    << " "
				<< p.energy
				<< "\n";
		}

		DOSM_LOG_INFO("Output written to output.txt");
	}

	void Factor::run(void)
	{
		DOSM_LOG_DEBUG("Factor::run() entered");
		DosmLawLennardJones::Result result;

		dosmParallel.init();
		dosmParallel.dispatch(1, [&](idx_t) { dosmLawLJ.kernel(&result); });
		dosmParallel.release();

		particles = *result.particles;
		DOSM_LOG_INFO("Lennard-Jones total energy = " + std::to_string(result.energy));

		outFile();

		DOSM_LOG_INFO("Factor::run() done");
	}

} // namespace dosm

