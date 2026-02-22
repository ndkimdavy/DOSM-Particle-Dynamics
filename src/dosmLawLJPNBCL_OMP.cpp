#include "dosmLawLJPNBCL_OMP.hpp"
#include "idosmSocket.hpp"
#include <omp.h>

#define STEP_SOCKET 10

namespace dosm
{
    DosmLawLJPNBCL_OMP::DosmLawLJPNBCL_OMP(vector_t<DosmParticle>& particles, 
        r64_t sigma, 
        r64_t epsilon, 
        r64_t boxLength, 
        r64_t rayCut, 
        r64_t skin, 
        idx_t dimX,
        idx_t dimY,
        idx_t stepEvery) : DosmLawLJPNB(particles, sigma, epsilon, boxLength, rayCut, skin, stepEvery), dimX(dimX), dimY(dimY)
    {
        subBoxLengthX = boxLength / (r64_t)dimX;
        subBoxLengthY = boxLength / (r64_t)dimY;
        grid.resize(dimX * dimY);
    }

    void DosmLawLJPNBCL_OMP::buildGrid(void)
    {
        for (auto& cell : grid)
            cell.clear();

        const idx_t n = particles.size();
        for (idx_t i = 0; i < n; ++i)
        {
            r64_t x = particles[i].position(0);
            r64_t y = particles[i].position(1);
            x -= boxLength * std::floor(x / boxLength);
            y -= boxLength * std::floor(y / boxLength);

            idx_t cellX = (idx_t)(x / subBoxLengthX);
            idx_t cellY = (idx_t)(y / subBoxLengthY);
            if (cellX >= dimX) cellX = dimX - 1;
            if (cellY >= dimY) cellY = dimY - 1;

            const idx_t cellId = cellY * dimX + cellX;
            grid[cellId].push_back(i);
        }
    }

    void DosmLawLJPNBCL_OMP::buildNeighborList(
        vector_t<vector_t<idx_t>>& neighbor,
        const vector_t<DosmParticle>& particles,
        const vector_t<tensor_t<r64_t, 3>>& images, 
        idx_t n,
        r64_t limit,
        idx_t maxNeighbor)
    {
        (void)images; // unused 
        buildGrid();

        neighbor.resize(n);
        for (idx_t i = 0; i < n; ++i)
        {
            neighbor[i].assign(1 + 2 * maxNeighbor, 0);
            neighbor[i][0] = 0; // no neighbor at the beginning
        }

        #pragma omp parallel for collapse(2) schedule(static)
        for (idx_t cellY = 0; cellY < dimY; ++cellY)
        for (idx_t cellX = 0; cellX < dimX; ++cellX)
        {
            const idx_t cellId = cellY * dimX + cellX;
            const auto& cell = grid[cellId];

            // Loop over particles i in this (owner) cell
            for (idx_t a = 0; a < (idx_t)cell.size(); ++a)
            {
                const idx_t i = cell[a];

                // Explore neighboring cells (including current cell) (3x3)
                for (int offsetY = -1; offsetY <= 1; ++offsetY)
                for (int offsetX = -1; offsetX <= 1; ++offsetX)
                {
                    idx_t neighborCellX = (idx_t)((int)cellX + offsetX);
                    idx_t neighborCellY = (idx_t)((int)cellY + offsetY);

                    // Periodicity on the cell grid
                    if ((int)neighborCellX < 0) neighborCellX += dimX;
                    if (neighborCellX >= dimX) neighborCellX -= dimX;
                    if ((int)neighborCellY < 0) neighborCellY += dimY;
                    if (neighborCellY >= dimY) neighborCellY -= dimY;

                    const idx_t neighborCellId = neighborCellY * dimX + neighborCellX;
                    const auto& neighborCell = grid[neighborCellId];

                    // Loop over candidate particles j in the neighboring cell
                    for (idx_t b = 0; b < (idx_t)neighborCell.size(); ++b)
                    {
                        const idx_t j = neighborCell[b];
                        if (j <= i) continue; // avoid double counting and self interaction

                        // Periodic boundary conditions
                        r64_t dx = particles[i].position(0) - particles[j].position(0);
                        r64_t dy = particles[i].position(1) - particles[j].position(1);
                        r64_t dz = particles[i].position(2) - particles[j].position(2);

                        dx -= boxLength * std::nearbyint(dx / boxLength);
                        dy -= boxLength * std::nearbyint(dy / boxLength);
                        dz -= boxLength * std::nearbyint(dz / boxLength);

                        const r64_t r2 = dx*dx + dy*dy + dz*dz;
                        if (r2 == 0.0 || r2 > limit) continue;

                        idx_t ni = neighbor[i][0] + 1;
                        if (ni > maxNeighbor) continue;

                        neighbor[i][0] = ni;
                        const idx_t mi = 2 * ni - 1;
                        neighbor[i][mi] = j;
                        neighbor[i][mi + 1] = 0; // dummy: no explicit periodic images stored
                    }
                }
            }
        }
    }

    void DosmLawLJPNBCL_OMP::kernel(Result* result)
    {
        if (!result) return;

        const idx_t n = particles.size();
        const r64_t rcut2  = rayCut * rayCut;
        const r64_t sigma2 = sigma * sigma;

        for (auto& particle : particles)
        {
            particle.p_energy = 0.0;
            particle.force(0) = particle.force(1) = particle.force(2) = 0.0;
        }

        // Build neighbor list (cell-list)
        if (neighbor.size() != n || ((stepEvery > 0) && ((stepCount++ % stepEvery) == 0)))
        {
            const r64_t rcutL  = rayCut + skin;
            const r64_t limit  = rcutL * rcutL;
            const r64_t rcutL3 = rcutL * rcutL * rcutL;

            const r64_t volume  = boxLength * boxLength * boxLength;
            const r64_t density = (volume > 0.0) ? ((r64_t)n / volume) : 0.0;
            const idx_t n_max_neighbor = ((idx_t)(density * 4.0 * M_PI * rcutL3)) * 2;

            buildNeighborList(neighbor, particles, images, n, limit, n_max_neighbor);
        }

        // Force / potential energy per OpenMP thread
        const idx_t nThreads = omp_get_max_threads();
        vector_t<r64_t> fx(nThreads * n, 0.0);
        vector_t<r64_t> fy(nThreads * n, 0.0);
        vector_t<r64_t> fz(nThreads * n, 0.0);
        vector_t<r64_t> pe(nThreads * n, 0.0);

        r64_t energy = 0.0;

        #pragma omp parallel reduction(+:energy)
        {
            const idx_t tid = (idx_t)omp_get_thread_num();

            r64_t* fx_thread = fx.data() + tid * n;
            r64_t* fy_thread = fy.data() + tid * n;
            r64_t* fz_thread = fz.data() + tid * n;
            r64_t* pe_thread = pe.data() + tid * n;

            #pragma omp for schedule(static)
            for (idx_t i = 0; i < n; ++i)
            {
                const idx_t nb = neighbor[i][0];
                
                for (idx_t k = 0; k < nb; ++k)
                {
                    const idx_t j = neighbor[i][1 + 2*k];

                    // Periodic boundary conditions
                    r64_t dx = particles[i].position(0) - particles[j].position(0);
                    r64_t dy = particles[i].position(1) - particles[j].position(1);
                    r64_t dz = particles[i].position(2) - particles[j].position(2);

                    dx -= boxLength * std::nearbyint(dx / boxLength);
                    dy -= boxLength * std::nearbyint(dy / boxLength);
                    dz -= boxLength * std::nearbyint(dz / boxLength);

                    const r64_t r2 = dx*dx + dy*dy + dz*dz;
                    if (r2 == 0.0 || r2 > rcut2) continue;

                    const r64_t invR2  = sigma2 / r2;
                    const r64_t invR6  = invR2 * invR2 * invR2;
                    const r64_t invR12 = invR6 * invR6;

                    const r64_t uij = 4.0 * epsilon * (invR12 - invR6);
                    energy += uij;

                    const r64_t oij  = 48.0 * epsilon * (invR12 - 0.5 * invR6);
                    const r64_t fxij = oij * (dx / r2);
                    const r64_t fyij = oij * (dy / r2);
                    const r64_t fzij = oij * (dz / r2);

                    // Accumulate forces in thread-local (no data race)
                    fx_thread[i] += fxij; fy_thread[i] += fyij; fz_thread[i] += fzij;
                    fx_thread[j] -= fxij; fy_thread[j] -= fyij; fz_thread[j] -= fzij;

                    // ui = uj = 1/2 uij (convention)
                    pe_thread[i] += 0.5 * uij;
                    pe_thread[j] += 0.5 * uij;
                }
            }
        }

        // Reduce thread-local forces and potential energies (no data races)
        for (idx_t i = 0; i < n; ++i)
        {
            r64_t fx_tot = 0.0, fy_tot = 0.0, fz_tot = 0.0, pe_tot = 0.0;

            for (idx_t tid = 0; tid < nThreads; ++tid)
            {
                fx_tot += fx[tid * n + i];
                fy_tot += fy[tid * n + i];
                fz_tot += fz[tid * n + i];
                pe_tot += pe[tid * n + i];
            }

            particles[i].force(0) += fx_tot;
            particles[i].force(1) += fy_tot;
            particles[i].force(2) += fz_tot;
            particles[i].p_energy += pe_tot;
        }

        result->energy = energy;
        result->particles = &particles;
    }

} // namespace dosm
