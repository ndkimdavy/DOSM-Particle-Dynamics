# DOSM — Molecular Dynamics Framework

DOSM is a modular C++ framework for molecular dynamics simulation of particle systems.

It implements several variants of the Lennard-Jones interaction model and provides configurable execution through a command-line interface.

---

## Features

- Particle-based system representation  
- Velocity-Verlet time integration  
- Lennard-Jones interactions  
- Periodic boundary conditions  
- Neighbor list (O(N))  
- Cell list implementation (O(N))  
- OpenMP parallelization  
- Configurable simulation parameters via CLI  
- CSV data export  
- PDB trajectory export (VMD compatible)  
- Optional socket-based live streaming  

---

## Visualization

Visualization of particle motion based on simulation data,
exported in PDB format for analysis with VMD.

![DOSM simulation preview](assets/v1.gif)
![DOSM simulation preview](assets/v2.gif)

---

## Interaction Laws

The following interaction models are available:

- `LJ` — basic Lennard-Jones  
- `LJP` — periodic Lennard-Jones  
- `LJPNB` — periodic + neighbor list  
- `LJPNBCL` — periodic + neighbor list + cell list (OpenMP)  

Select using:

```bash
--law LJPNBCL
```

---

## Build

```bash
cmake -S . -B build
cmake --build build
```

---

## Run

Display available options:

```bash
./build/dosmetric --help
```

Example execution:

```bash
OMP_NUM_THREADS=8 ./build/dosmetric \
--law LJPNBCL \
--input input/particule \
--csv out.csv \
--pdb out.pdb \
--seed 42 \
--mass 18.0 \
--charge 0.0 \
--sigma 3.0 \
--epsilon 0.2 \
--length 42.0 \
--rcut 10.0 \
--skin 0.0 \
--dt 1.0 \
--steps 10000 \
--gridx 4 \
--gridy 4 \
--stepevery 10 \
--stepsocket 5 \
--ip 127.0.0.1 \
--port 5555
```

---

## Output

- `dosmdata.csv` — time evolution of positions, momenta, forces, energies, temperature  
- `dosmvisual.pdb` — trajectory file for visualization (e.g. VMD)  

---

## Parallel Execution

OpenMP parallelism can be controlled via:

```bash
OMP_NUM_THREADS=8 ./build/dosmetric ...
```

---

## Project Status

The current implementation supports scalable short-range molecular dynamics simulations using CPU parallelism.

Future work may include:

- Automatic grid tuning  
- Performance benchmarking  
- Additional interaction models  
- Extended validation tools  