#include "dosmLawVV.hpp"
#include "idosmSocket.hpp"

#define CONVERSION_FORCE (0.0001 * 4.186)
#define CONSTANT_R       0.00199
#define T0               300.0
#define RANDOM_UNIT()    (rand() / (r64_t)RAND_MAX)
#define RANDOM_SIGN()    ((RANDOM_UNIT() < 0.5) ? -1.0 : 1.0)
#define GAMMA 0.01
#define STEP_THERMO 10
#define STEP_SOCKET 1

namespace dosm
{
    DosmLawVV::DosmLawVV(IDosmLaw& idosmLaw, DosmParticleSnap::Snap& snap, r64_t dt, r64_t boxLength):
        idosmLaw(idosmLaw), 
        snap(snap)
    {
        this->dt = dt;
        this->boxLength = boxLength;
        init();
    }

    void DosmLawVV::kernel(Result* result)
    {
        if (!result) return;

        // F(t)
        idosmLaw.kernel(result);

        // p(t+dt/2), r(t+dt)
        for (auto& particle : snap.particles)
        {
            particle.momentum = particle.momentum + 0.5 * dt * (CONVERSION_FORCE * particle.force);
            particle.position = particle.position + dt * (particle.momentum / particle.mass);
            // r(t+dt) = r(t+dt) - aint(r(t+dt)/L)*L 
            particle.position(0) = particle.position(0) - std::nearbyint(particle.position(0) / boxLength) * boxLength;
            particle.position(1) = particle.position(1) - std::nearbyint(particle.position(1) / boxLength) * boxLength;
            particle.position(2) = particle.position(2) - std::nearbyint(particle.position(2) / boxLength) * boxLength;
        }

        // F(t+dt)
        idosmLaw.kernel(result);

        // p(t+dt)
        for (auto& particle : snap.particles)
            particle.momentum = particle.momentum + 0.5 * dt * (CONVERSION_FORCE * particle.force);

        // Ek
        r64_t Ek = 0.0;
        for (auto& particle : snap.particles)
        {
            particle.k_energy = (1.0 / (2.0 * CONVERSION_FORCE * particle.mass)) * (particle.momentum(0) * particle.momentum(0) + particle.momentum(1) * particle.momentum(1) + particle.momentum(2) * particle.momentum(2));
            Ek += particle.k_energy;
        }

        // T
        const idx_t N   = snap.particles.size();
        const idx_t Ndl = 3 * N - 3;
        r64_t T = (Ndl > 0) ? (Ek / (Ndl * CONSTANT_R)) : 0.0;

        // THERMO BERENDSEN
        if (stepCount++ % STEP_THERMO == 0 && T > 0.0)
        {
            r64_t lambda = 1.0 + GAMMA * (T0 / T - 1.0);

            for (auto& particle : snap.particles)
                particle.momentum = particle.momentum * lambda;

            tensor_t<r64_t, 3> pc = {0.0, 0.0, 0.0};
            for (auto& particle : snap.particles)
                pc = pc + particle.momentum;

            for (auto& particle : snap.particles)
                particle.momentum = particle.momentum - pc / (r64_t)N;

            Ek = 0.0;
            for (auto& particle : snap.particles)
            {
                particle.k_energy = (1.0 / (2.0 * CONVERSION_FORCE * particle.mass)) * (particle.momentum(0) * particle.momentum(0) + particle.momentum(1) * particle.momentum(1) + particle.momentum(2) * particle.momentum(2));
                Ek += particle.k_energy;
            }

            T = (Ndl > 0) ? (Ek / (Ndl * CONSTANT_R)) : 0.0;
        }

        static idx_t socketCount = 0;
        if (result->idosmSocket && !(socketCount++ % STEP_SOCKET))
        {
            const r64_t Ep   = result->energy;
            const r64_t Etot = Ek + Ep;
            chr_t data[256];
            i32_t len = snprintf(data, sizeof(data), "VV\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\n", snap.t, Ek, Ep, Etot, T);
            if (len > 0) 
                result->idosmSocket->send(data, (idx_t)len);
        }

        snap.t = snap.t + dt;
    }

    void DosmLawVV::init()
    {
        const idx_t N   = snap.particles.size();
        const idx_t Ndl = 3 * N - 3;

        for (auto& particle : snap.particles)
        {
            particle.velocity(0) = RANDOM_SIGN() * RANDOM_UNIT();
            particle.velocity(1) = RANDOM_SIGN() * RANDOM_UNIT();
            particle.velocity(2) = RANDOM_SIGN() * RANDOM_UNIT();
            particle.momentum = particle.mass * particle.velocity;
        }

        tensor_t<r64_t, 3> pc = {0.0, 0.0, 0.0};
        for (auto& particle : snap.particles)
            pc = pc + particle.momentum;

        for (auto& particle : snap.particles)
            particle.momentum = particle.momentum - pc / (r64_t)N;

        r64_t Ek_init = 0.0;
        for (auto& particle : snap.particles)
            Ek_init += (1.0 / (2.0 * CONVERSION_FORCE * particle.mass)) * (particle.momentum(0) * particle.momentum(0) + particle.momentum(1) * particle.momentum(1) + particle.momentum(2) * particle.momentum(2));

        const r64_t RAPPORT = (Ndl * CONSTANT_R * T0) / Ek_init;
        const r64_t scale   = std::sqrt(RAPPORT);

        for (auto& particle : snap.particles)
            particle.momentum = particle.momentum * scale;

        // F(0), Ep(0)
        IDosmLaw::Result result;
        idosmLaw.kernel(&result);
        // Ek(0)
        for (auto& particle : snap.particles)
            particle.k_energy = (1.0 / (2.0 * CONVERSION_FORCE * particle.mass)) * (particle.momentum(0) * particle.momentum(0) + particle.momentum(1) * particle.momentum(1) + particle.momentum(2) * particle.momentum(2));
    }

} // namespace dosm
