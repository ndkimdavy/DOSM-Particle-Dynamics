#ifndef DOSM_CONFIG_HPP
#define DOSM_CONFIG_HPP

#include "dosmBasic.hpp"

namespace dosm
{
    struct DosmConfig
    {
        str_t law        = "LJPNBCL";
        str_t inputFile  = "input.txt";
        str_t mciFile    = "examen_270226_mci";
        str_t csvFile    = "dosmdata.csv";
        str_t pdbFile    = "dosmvisual.pdb";
        ui32_t seed      = 42;
        r64_t mass       = 18.0;
        r64_t charge     = 0.0;
        r64_t sigma      = 3.0;
        r64_t epsilon    = 0.2;
        r64_t boxLength  = 42.0;
        r64_t rayCut     = 10.0;
        r64_t skin       = 0.0;
        r64_t dt         = 1.0;
        idx_t steps      = 5000;
        idx_t gridDimX   = 16;
        idx_t gridDimY   = 16;
        idx_t stepEvery  = 10;
        bool thermostat  = false;
        idx_t stepSocket = 10;
        str_t ip         = "127.0.0.1";
        ui16_t port      = 5555;
    };

    inline DosmConfig config;

} // namespace dosm

#endif // DOSM_CONFIG_HPP