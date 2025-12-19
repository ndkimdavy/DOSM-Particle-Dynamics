#include "dosmLawLennardJones.hpp"
#include <cmath>

namespace dosm
{
	DosmLawLennardJones::DosmLawLennardJones(vector_t<DosmParticle> particles, r64_t sigma, r64_t epsilon)
	{
		this->sigma = sigma;
		this->epsilon = epsilon;
		this->particles = particles;
	}

	void DosmLawLennardJones::kernel(Result* result)
	{
		idx_t n = particles.size();

		r64_t energy = 0.0;
		for (idx_t i = 0; i < n; ++i)
		{
			particles[i].energy   = 0.0;
			particles[i].force(0) = 0.0;
			particles[i].force(1) = 0.0;
			particles[i].force(2) = 0.0;
		}

		for (idx_t i = 0; i < n; ++i)
		{
			for (idx_t j = i + 1; j < n; ++j)
			{
				r64_t dx = particles[j].position(0) - particles[i].position(0);
				r64_t dy = particles[j].position(1) - particles[i].position(1);
				r64_t dz = particles[j].position(2) - particles[i].position(2);
				r64_t r2 = dx*dx + dy*dy + dz*dz;
				r64_t sigma2 = sigma * sigma;

				r64_t uij = 4.0 * epsilon * (std::pow(sigma2/r2, 6) - std::pow(sigma2/r2, 3));
				energy += uij;

				r64_t oij = 48.0 * epsilon * (std::pow(sigma2/r2, 6) - 0.5 * std::pow(sigma2/r2, 3));
				r64_t fx = oij * (dx/r2);
				r64_t fy = oij * (dy/r2);
				r64_t fz = oij * (dz/r2);

				particles[i].force(0) += fx;
				particles[i].force(1) += fy;
				particles[i].force(2) += fz;

				particles[j].force(0) -= fx;
				particles[j].force(1) -= fy;
				particles[j].force(2) -= fz;

				// ui = uj = 1/2 uij (convention)
				particles[i].energy += 0.5 * uij;
				particles[j].energy += 0.5 * uij;
			}

			DOSM_PROGRESS("Lennard-Jones", i + 1, n);
		}

		result->energy    = energy;
		result->particles = &particles;

	}

} // namespace dosm

