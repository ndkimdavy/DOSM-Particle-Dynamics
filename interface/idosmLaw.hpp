#ifndef IDOSM_LAW_HPP
#define IDOSM_LAW_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"

namespace dosm
{
    class IDosmLaw
    {
        public:
            struct Plot 
            {
                vector_t <r64_t> x;
                vector_t <r64_t> y;
                vector_t <r64_t> z;
            };

            struct Result
            {
                r64_t energy = 0.0;    
                vector_t<DosmParticle>* particles = nullptr;
                DosmParticleSnap::Snap* snap =  nullptr;
                Plot* plot = nullptr; 
            };

            virtual ~IDosmLaw(void) = default;
            virtual void kernel(Result* result) = 0;
    };

} // namespace dosm

#endif // IDOSM_LAW_HPP


