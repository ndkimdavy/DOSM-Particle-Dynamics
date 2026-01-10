#ifndef IDOSM_LAW_HPP
#define IDOSM_LAW_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"

namespace dosm
{
    class IDosmSocket;

    class IDosmLaw
    {
        public:
            struct Result
            {
                r64_t energy = 0.0;    
                vector_t<DosmParticle>* particles = nullptr;
                DosmParticleSnap::Snap* snap =  nullptr;
                IDosmSocket* idosmSocket = nullptr;
            };

            virtual ~IDosmLaw(void) = default;
            virtual void kernel(Result* result) = 0;
    };

} // namespace dosm

#endif // IDOSM_LAW_HPP


