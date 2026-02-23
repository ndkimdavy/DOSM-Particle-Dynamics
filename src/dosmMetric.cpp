#include "dosmMetric.hpp"
#include "dosmLawVV.hpp"
#include "dosmLawLJPNB.hpp"
#include "dosmLawLJPNBCL_OMP.hpp"
#include "dosmSocketPublisher.hpp"

#include <cstdio>
#include <chrono>
#include <fstream>
#include <stdexcept>

namespace dosm
{


    static inline dosm::r64_t min_image(dosm::r64_t dx, dosm::r64_t L)
    {
        dx -= L * std::round(dx / L);
        return dx;
    }

    static dosm::r64_t computeMeanNeighbors(
        const dosm::vector_t<dosm::DosmParticle>& particles,
        dosm::r64_t L,
        dosm::r64_t Rc)
    {
        const dosm::idx_t N = particles.size();
        const dosm::r64_t Rc2 = Rc * Rc;

        dosm::r64_t sumCount = 0.0;

        for (dosm::idx_t i = 0; i < N; ++i)
        {
            dosm::r64_t count = 0.0;
            const auto& pi = particles[i].position;

            for (dosm::idx_t j = 0; j < N; ++j)
            {
                if (j == i) continue;

                dosm::r64_t dx = pi(0) - particles[j].position(0);
                dosm::r64_t dy = pi(1) - particles[j].position(1);
                dosm::r64_t dz = pi(2) - particles[j].position(2);

                dx = min_image(dx, L);
                dy = min_image(dy, L);
                dz = min_image(dz, L);

                const dosm::r64_t r2 = dx*dx + dy*dy + dz*dz;
                if (r2 < Rc2) count += 1.0;
            }

            sumCount += count;
        }

        return sumCount / (dosm::r64_t)N; // M(Rc)
    }


    DosmMetric::DosmMetric(void)
    {
        std::srand(config.seed);
        loadFile(config.inputFile);
    }

    void DosmMetric::loadFile(const str_t& file)
    {
        std::ifstream in(file);
        if (!in)
        {
            DOSM_LOG_ERROR("Cannot open file: " + file);
            throw std::runtime_error("dosmMetric: cannot open file");
        }

        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        vector_t<DosmParticle> particles;
        i32_t type;
        r64_t x, y, z;
        while (in >> type >> x >> y >> z)
        {
            DosmParticle particle(config.mass, config.charge);
            particle.position(0) = x;
            particle.position(1) = y;
            particle.position(2) = z;
            particle.velocity(0) = particle.velocity(1) = particle.velocity(2) = 0.0;
            particles.push_back(particle);
        }

        dosmParticleSnap.snaps.clear();
        dosmParticleSnap.snaps.push_back(DosmParticleSnap::Snap{0.0, particles});
        DOSM_LOG_INFO("Loaded " + std::to_string(particles.size()) + " particles");
    }

    void DosmMetric::loadFileExam(const str_t& xyzFile, const str_t& mciFile)
    {
        // -----------------------
        // 1) Lire XYZ (positions)
        // -----------------------
        std::ifstream fin(xyzFile);
        if (!fin) throw std::runtime_error("DosmMetric::loadFileExam: cannot open xyz file: " + xyzFile);

        fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip header line

        vector_t<DosmParticle> particles;
        particles.reserve(1000);

        idx_t id;
        r64_t x, y, z;

        while (fin >> id >> x >> y >> z)
        {
            DosmParticle p(config.mass, config.charge);

            p.position(0) = x;
            p.position(1) = y;
            p.position(2) = z;

            // init propre
            p.momentum(0) = p.momentum(1) = p.momentum(2) = 0.0;
            p.velocity(0) = p.velocity(1) = p.velocity(2) = 0.0;
            p.force(0)    = p.force(1)    = p.force(2)    = 0.0;
            p.k_energy = 0.0;
            p.p_energy = 0.0;

            particles.push_back(p);
        }

        if (particles.empty())
            throw std::runtime_error("DosmMetric::loadFileExam: xyz empty or bad format: " + xyzFile);

        // -----------------------
        // 2) Lire MCI (moments)
        // -----------------------
        std::ifstream fmi(mciFile);
        if (!fmi) throw std::runtime_error("DosmMetric::loadFileExam: cannot open mci file: " + mciFile);

        fmi.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip header line

        idx_t mid;
        r64_t px, py, pz;

        idx_t i = 0;
        while (fmi >> mid >> px >> py >> pz)
        {
            if (i >= particles.size())
                throw std::runtime_error("DosmMetric::loadFileExam: mci has more lines than xyz");

            particles[i].momentum(0) = px;
            particles[i].momentum(1) = py;
            particles[i].momentum(2) = pz;

            // Q1.b : v = p/m
            particles[i].velocity(0) = px / config.mass;
            particles[i].velocity(1) = py / config.mass;
            particles[i].velocity(2) = pz / config.mass;

            ++i;
        }

        if (i != particles.size())
            throw std::runtime_error("DosmMetric::loadFileExam: mci lines != xyz lines");

        // -----------------------
        // 3) Snap t=0
        // -----------------------
        dosmParticleSnap.snaps.clear();
        dosmParticleSnap.snaps.push_back({0.0, particles});
    }


    void DosmMetric::outFile(void)
    {
        const str_t csvFile = config.csvFile;
        const str_t pdbFile = config.pdbFile;

        std::ofstream csv(csvFile);
        if (!csv)
        {
            DOSM_LOG_ERROR("Cannot open CSV file: " + csvFile);
            return;
        }

        std::ofstream pdb(pdbFile);
        if (!pdb)
        {
            DOSM_LOG_ERROR("Cannot open PDB file: " + pdbFile);
            return;
        }

        chr_t line[1024];

        snprintf(
                line,
                sizeof(line),
                "snap\tt\tid\tmass\tx\ty\tz\tpx\tpy\tpz\tfx\tfy\tfz\tEk\tEp\tT\n"
                );
        csv << line;


        snprintf(line, sizeof(line), "HEADER    DOSM PARTICLES\n");
        pdb << line;

        snprintf(line, sizeof(line), "REMARK    Generated by dosmetric\n");
        pdb << line;

        snprintf(
                line,
                sizeof(line),
                "CRYST1%9.3f%9.3f%9.3f  90.00  90.00  90.00 P 1\n",
                config.boxLength, config.boxLength, config.boxLength
                );
        pdb << line;

        const idx_t nSnap = dosmParticleSnap.snaps.size();

        for (idx_t i = 0; i < nSnap; ++i)
        {
            const auto& snap = dosmParticleSnap.snaps[i];
            const idx_t ntasks = snap.particles.size();
            const idx_t Ndl = 3 * ntasks - 3;
            constexpr r64_t CONSTANT_R = 0.00199;
            r64_t Ec = 0.0;
            for (const auto& particle : snap.particles)
                Ec += particle.k_energy;

            r64_t T = (Ndl > 0) ? (Ec / (Ndl * CONSTANT_R)) : 0.0;
            snprintf(
                    line,
                    sizeof(line),
                    "MODEL     %d\n",
                    i + 1
                    );
            pdb << line;

            const idx_t n = snap.particles.size();
            for (idx_t j = 0; j < n; ++j)
            {
                const DosmParticle& particle = snap.particles[j];

                snprintf(
                        line,
                        sizeof(line),
                        "%d\t%.17g\t%d\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\t%.17g\n",
                        i,
                        snap.t,
                        j,
                        particle.mass,
                        particle.position(0), particle.position(1), particle.position(2),
                        particle.momentum(0), particle.momentum(1), particle.momentum(2),
                        particle.force(0),    particle.force(1),    particle.force(2),
                        particle.k_energy,
                        particle.p_energy,
                        T
                        );
                csv << line;

                snprintf(
                        line,
                        sizeof(line),
                        "HETATM%5d  %-3.3s %-3.3s %4d    %8.3f%8.3f%8.3f%6.2f%6.2f           %.2s\n",
                        j + 1,
                        "X",
                        "SIM",
                        1,
                        particle.position(0), particle.position(1), particle.position(2),
                        1.00,
                        particle.p_energy,
                        "X"
                        );

                pdb << line;
            }

            snprintf(line, sizeof(line), "ENDMDL\n");
            pdb << line;

            DOSM_PROGRESS("Generating output", i + 1, nSnap, -1);
        }

        DOSM_LOG_INFO("Generated outputs: " + csvFile + ", " + pdbFile);
    }

    void DosmMetric::run(void)
    {
        DOSM_LOG_DEBUG("Factor::run() entered");


        // Charger les fichiers examen
        loadFileExam(config.inputFile, config.mciFile);
        //  Routine Q2 : M(Rc) avant LJ/VV
        {
            const auto& particles0 = dosmParticleSnap.snaps[0].particles;

            std::ofstream outM("M_vs_Rc.dat");
            for (r64_t Rc : {8.0, 10.0, 12.0, 14.0})
            {
                const r64_t M = computeMeanNeighbors(particles0, config.boxLength, Rc);
                outM << Rc << " " << M << "\n";
                std::cout << "M(" << Rc << ") = " << M << "\n";
            }
        }

        // ------------------------------------------------
        // Q4 : sommes des vitesses + température cinétique
        // ------------------------------------------------
        {
            const auto& particles0 = dosmParticleSnap.snaps[0].particles;
            const idx_t N = particles0.size();
            const idx_t Ndf = 3 * N - 3;

            const r64_t R_exam = 8.31e-7; 

            r64_t sumVx = 0.0, sumVy = 0.0, sumVz = 0.0;
            r64_t sumV2 = 0.0;

            for (const auto& p : particles0)
            {
                const r64_t vx = p.velocity(0);
                const r64_t vy = p.velocity(1);
                const r64_t vz = p.velocity(2);

                sumVx += vx;
                sumVy += vy;
                sumVz += vz;

                sumV2 += (vx*vx + vy*vy + vz*vz);
            }

            // Energie cinétique : K = 1/2 m Σ v^2
            const r64_t K = 0.5 * config.mass * sumV2;

            // Température cinétique : T = 2K / (Ndf * R)
            const r64_t T = (Ndf > 0) ? (2.0 * K) / ( (r64_t)Ndf * R_exam ) : 0.0;

            std::cout << "Q4: sum(vx)=" << sumVx
                    << " sum(vy)=" << sumVy
                    << " sum(vz)=" << sumVz << "\n";
            std::cout << "Q4: K=" << K << "  Ndf=" << Ndf << "  T=" << T << "\n";
        }

        // --------------------------
        // Q5 : correction centre de masse
        // --------------------------
        {
            auto& particles0 = dosmParticleSnap.snaps[0].particles;
            const idx_t N = particles0.size();

            r64_t sumVx = 0.0, sumVy = 0.0, sumVz = 0.0;

            for (const auto& p : particles0)
            {
                sumVx += p.velocity(0);
                sumVy += p.velocity(1);
                sumVz += p.velocity(2);
            }

            const r64_t vcmx = sumVx / (r64_t)N;
            const r64_t vcmy = sumVy / (r64_t)N;
            const r64_t vcmz = sumVz / (r64_t)N;

            std::cout << "Q5: vcm = ("
                    << vcmx << ", "
                    << vcmy << ", "
                    << vcmz << ")\n";

            // soustraction
            for (auto& p : particles0)
            {
                p.velocity(0) -= vcmx;
                p.velocity(1) -= vcmy;
                p.velocity(2) -= vcmz;

                // cohérence momentum
                p.momentum = p.mass * p.velocity;
            }

            // vérification
            r64_t checkX=0, checkY=0, checkZ=0;
            for (const auto& p : particles0)
            {
                checkX += p.velocity(0);
                checkY += p.velocity(1);
                checkZ += p.velocity(2);
            }

            std::cout << "Q5 after correction: "
                    << checkX << " "
                    << checkY << " "
                    << checkZ << "\n";
        }


        IDosmLaw::Result result;

        DosmParticleSnap::Snap currSnap = dosmParticleSnap.snaps.back();

        for (auto& particle : currSnap.particles)
            particle.mass = config.mass;
        
        baseLaw.reset();
        if(config.law == "LJ")
            baseLaw = std::make_unique<DosmLawLJ>(currSnap.particles, config.sigma, config.epsilon);
        else if(config.law == "LJP")
            baseLaw = std::make_unique<DosmLawLJP>(currSnap.particles, config.sigma, config.epsilon, config.boxLength, config.rayCut);
        else if(config.law == "LJPNB")
            baseLaw = std::make_unique<DosmLawLJPNB>(currSnap.particles, config.sigma, config.epsilon, config.boxLength, config.rayCut, config.skin);
        else if(config.law == "LJPNBCL")
            baseLaw = std::make_unique<DosmLawLJPNBCL_OMP>(currSnap.particles, config.sigma, config.epsilon, config.boxLength, config.rayCut, config.skin, config.gridDimX, config.gridDimY);
        else
        {
            DOSM_LOG_ERROR("Unknown law: " + config.law);
            throw std::runtime_error("dosmMetric: unknown law");
        }

        idosmLaw = std::make_unique<DosmLawVV>(*baseLaw, currSnap, config.dt, config.boxLength);
        dosmParticleSnap.snaps[0] = currSnap;
        idosmSocket = std::make_unique<DosmSocketPublisher>(config.ip, config.port);
        idosmSocket->init();

        auto t0 = std::chrono::steady_clock::now();
        for (idx_t step = 1; step < config.steps; ++step)
        {
            result.idosmSocket = idosmSocket.get();
            idosmLaw->kernel(&result);
            dosmParticleSnap.snaps.push_back(currSnap);
            ui64_t elapsed = (ui64_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - t0).count();
            DOSM_PROGRESS("Time step", step + 1, config.steps, elapsed);
        }

        // ------------------------------------------------
        // Q8 (EXAM) : g(T) sur [200,500], dT=5
        // T(t) calculée avec R = 8.31e-7 et Ndf = 3N-3
        // ------------------------------------------------
        {
            const r64_t Tmin = 200.0;
            const r64_t Tmax = 500.0;
            const r64_t dT   = 5.0;
            const idx_t nbins = (idx_t)((Tmax - Tmin) / dT); // 60

            vector_t<idx_t> hist(nbins, 0);

            const r64_t R_exam = 8.31e-7;
            const r64_t CONV_FORCE = (0.0001 * 4.186); 

            r64_t TminObs = 1e300;
            r64_t TmaxObs = -1e300;

            for (const auto& snap : dosmParticleSnap.snaps)
            {
                const idx_t N   = snap.particles.size();
                const idx_t Ndf = 3 * N - 3;
                if (Ndf <= 0) continue;

                // Ek(snapshot) depuis les moments cinétiques
                r64_t Ek = 0.0;
                for (const auto& p : snap.particles)
                {
                    const r64_t px = p.momentum(0);
                    const r64_t py = p.momentum(1);
                    const r64_t pz = p.momentum(2);
                    Ek += (1.0 / (2.0 * CONV_FORCE * p.mass)) * (px*px + py*py + pz*pz);
                }

                // Température examen : Ek = Ndf * R * T
                const r64_t T = Ek / ((r64_t)Ndf * R_exam);

                TminObs = std::min(TminObs, T);
                TmaxObs = std::max(TmaxObs, T);

                if (T >= Tmin && T < Tmax)
                {
                    const idx_t bin = (idx_t)((T - Tmin) / dT);
                    if (bin < nbins) hist[bin]++;
                }
            }

            std::cout << "Q8(EXAM): TminObs=" << TminObs << " TmaxObs=" << TmaxObs << "\n";

            std::ofstream out("gT.dat");
            for (idx_t b = 0; b < nbins; ++b)
            {
                const r64_t Tcenter = Tmin + (b + 0.5) * dT;
                out << Tcenter << " " << hist[b] << "\n";
            }
        }
            idosmSocket->release();

        outFile();

        DOSM_LOG_INFO("Factor::run() done");
    }

} // namespace dosm
