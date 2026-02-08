#include "dosmLawLJPNB_OMP.hpp"
#include "idosmSocket.hpp" 
#include <omp.h>

#define STEP_SOCKET 10

namespace dosm
{
    DosmLawLJPNB_OMP::DosmLawLJPNB_OMP(
        vector_t<DosmParticle>& particles, 
        r64_t sigma, 
        r64_t epsilon, 
        r64_t boxLength, 
        r64_t rayCut, 
        r64_t skin, 
        idx_t stepEvery) : DosmLawLJPNB(particles, sigma, epsilon, boxLength, rayCut, skin, stepEvery)
    {}

    void DosmLawLJPNB_OMP::kernel(Result* result)
    {
        if (!result) return;

        const idx_t n = particles.size();
        const r64_t rcut2  = rayCut * rayCut;
        const r64_t rcut3  = rayCut * rayCut * rayCut;
        const r64_t rcutL  = rayCut + skin;
        const r64_t rcutL2 = rcutL * rcutL;
        const r64_t rcutL3 = rcutL * rcutL * rcutL;
        const r64_t sigma2 = sigma * sigma;

        for (auto& particle : particles)
        {
            particle.p_energy = 0.0;
            particle.force(0) = particle.force(1) = particle.force(2) = 0.0;
        }

        // Build neighbor list
        if ((stepEvery > 0) && ((stepCount++ % stepEvery) == 0))
        {
            const r64_t volume  = boxLength * boxLength * boxLength;
            const r64_t density = (volume > 0.0) ? ((r64_t)n / volume) : 0.0;
            const idx_t n_max_neighbor = ((idx_t)(density * 4.0 * M_PI * rcutL3)) * 2;

            neighbor.resize(n);
            for (idx_t i = 0; i < n; ++i)
            {
                neighbor[i].assign(1 + 2 * n_max_neighbor, 0);
                neighbor[i][0] = 0;
            }

            for (idx_t i = 0; i < n; ++i)
                for (idx_t j = i + 1; j < n; ++j)
                    for (idx_t isym = 0; isym < (idx_t)images.size(); ++isym)
                    {
                        const auto& image = images[isym];

                        const r64_t dx = particles[i].position(0) - (particles[j].position(0) + image(0));
                        const r64_t dy = particles[i].position(1) - (particles[j].position(1) + image(1));
                        const r64_t dz = particles[i].position(2) - (particles[j].position(2) + image(2));
                        const r64_t r2 = dx*dx + dy*dy + dz*dz;
                        if (r2 == 0.0 || r2 > rcutL2) continue;

                        idx_t ni = neighbor[i][0] + 1;
                        if (ni > n_max_neighbor) continue; 

                        neighbor[i][0] = ni;
                        const idx_t mi = 2 * ni - 1;
                        neighbor[i][mi] = j;
                        neighbor[i][mi + 1] = isym;
                    }
        }

        r64_t energy = 0.0;
        #pragma omp parallel for reduction(+:energy) schedule(static)
        for (idx_t i = 0; i < n; ++i)
        {
            r64_t fx_i = 0.0;
            r64_t fy_i = 0.0;
            r64_t fz_i = 0.0;
            r64_t p_energy_i = 0.0;
            const idx_t nb = neighbor[i][0];

            for (idx_t k = 0; k < nb; ++k)
            {
                const idx_t j = neighbor[i][1 + 2*k];
                const idx_t isym = neighbor[i][1 + 2*k + 1];
                const auto& image = images[isym];

                const r64_t dx = particles[i].position(0) - (particles[j].position(0) + image(0));
                const r64_t dy = particles[i].position(1) - (particles[j].position(1) + image(1));
                const r64_t dz = particles[i].position(2) - (particles[j].position(2) + image(2));
                const r64_t r2 = dx*dx + dy*dy + dz*dz;
                if (r2 == 0.0 || r2 > rcut2) continue;

                const r64_t _invR2 = sigma2 / r2;
                const r64_t _invR6 = _invR2 * _invR2 * _invR2;
                const r64_t _invR12 = _invR6 * _invR6;

                const r64_t uij = 4.0 * epsilon * (_invR12 - _invR6);
                energy += uij;
                p_energy_i += 0.5 * uij;

                // static idx_t socketCount = 0;
                // if (result->idosmSocket && !(socketCount++ % STEP_SOCKET))
                // {
                //     const r64_t r = std::sqrt(r2);
                //     chr_t data[256];
                //     i32_t len = snprintf(data, sizeof(data), "LJ\t%.17g\t%.17g\n", r / sigma, uij);
                //     if (len > 0)
                //         result->idosmSocket->send(data, (idx_t)len);
                // }

                const r64_t oij = 48.0 * epsilon * (_invR12 - 0.5 * _invR6);
                fx_i += oij * (dx/r2);
                fy_i += oij * (dy/r2);
                fz_i += oij * (dz/r2);
            }

            particles[i].force(0) += fx_i;
            particles[i].force(1) += fy_i;
            particles[i].force(2) += fz_i;
            particles[i].p_energy += p_energy_i;

            // DOSM_PROGRESS("Lennard-Jones Periodic (NB)", i + 1, n, -1);
        }

        result->energy = energy;
        result->particles = &particles;
    }

} // namespace dosm
