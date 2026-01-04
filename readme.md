# DOSM-Molecular-Dynamics

DOSM-Molecular-Dynamics is a lightweight C++ framework for numerical simulation of particle systems.

The project implements the core elements of a molecular dynamics workflow, including:
- Particle-based system representation
- Time integration with a velocity-Verlet scheme
- Short-range interactions
- Periodic boundary conditions
- Basic thermodynamic quantities (energy, temperature)
- Trajectory export for visualization (e.g. VMD)

The code is designed for experimentation and learning, and is structured to allow future extensions.

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/dosmetric
```

This generates:
- `dosmdata.csv` — numerical data per time step (positions, momenta, energies, temperature)
- `dosmvisual.pdb` — trajectory file for visualization (e.g. with VMD)

## Project status

This project is a work in progress.
The current focus is on validating physical behavior and numerical consistency.

Possible future directions include:
- Performance improvements (vectorization, parallelism)
- Additional interaction models
- Extended analysis tools
- Visualization and rendering experiments
