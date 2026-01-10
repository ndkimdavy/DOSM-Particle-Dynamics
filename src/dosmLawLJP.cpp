#include "dosmLawLJP.hpp"
#include "idosmSocket.hpp" 

namespace dosm
{
    DosmLawLJP::DosmLawLJP(vector_t<DosmParticle>& particles, r64_t sigma, r64_t epsilon, r64_t boxLength, r64_t rayCut) : 
        DosmLawLJ(particles, sigma, epsilon)  
    {
        this->boxLength = boxLength;
        this->rayCut = rayCut;
        images.reserve(27);
        for (i32_t i = -1; i <= 1; ++i)
            for (i32_t j = -1; j <= 1; ++j) 
                for (i32_t k = -1; k <= 1; ++k)
                {
                    tensor_t<r64_t, 3> image;
                    image(0) = i * boxLength;
                    image(1) = j * boxLength;
                    image(2) = k * boxLength;
                    images.push_back(image);
                }
    }

    void DosmLawLJP::kernel(Result* result)
    {
        if(!result) return;

        idx_t n = particles.size();

        r64_t energy = 0.0;
        for (auto& particle : particles)
        {
            particle.p_energy = 0.0;
            particle.force(0) = particle.force(1) = particle.force(2) = 0.0;
        }

        for (auto& image : images)
            for (idx_t i = 0; i < n; ++i)
            {
                for (idx_t j = i + 1; j < n; ++j)
                {
                    r64_t dx = particles[i].position(0) - (particles[j].position(0) + image(0));
                    r64_t dy = particles[i].position(1) - (particles[j].position(1) + image(1));
                    r64_t dz = particles[i].position(2) - (particles[j].position(2) + image(2));
                    r64_t r2 = dx*dx + dy*dy + dz*dz;
                    if (r2 == 0.0 || r2 > rayCut * rayCut) continue;
                    r64_t sigma2 = sigma * sigma;

                    r64_t _invR2  = sigma2 / r2;
                    r64_t _invR6  = _invR2 * _invR2 * _invR2;
                    r64_t _invR12 = _invR6 * _invR6;

                    r64_t uij = 4.0 * epsilon * (_invR12 - _invR6);
                    energy += uij;

                    r64_t r = std::sqrt(r2);
                    static idx_t counter = 0;
                    if (result->idosmSocket && !(counter++ % 1000))
                    {
                        chr_t data[256];
                        i32_t len = snprintf(data, sizeof(data), "LJ\t%.17g\t%.17g\n", r / sigma, uij);
                        if (len > 0) 
                            result->idosmSocket->send(data, (idx_t)len);
                    }

                    r64_t oij = 48.0 * epsilon * (_invR12 - 0.5 * _invR6);
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
                    particles[i].p_energy += 0.5 * uij;
                    particles[j].p_energy += 0.5 * uij;
                }

                // DOSM_PROGRESS("Lennard-Jones Periodic", i + 1, n, -);
            }

        result->energy    = energy;
        result->particles = &particles;

    }

} // namespace dosm
