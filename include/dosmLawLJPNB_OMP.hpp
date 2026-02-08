#ifndef DOSM_LAW_LJPNB_OMP_HPP
#define DOSM_LAW_LJPNB_OMP_HPP

#include "dosmLawLJPNB.hpp"

namespace dosm
{
    class DosmLawLJPNB_OMP : public DosmLawLJPNB
    {
        public:
            DosmLawLJPNB_OMP(
                vector_t<DosmParticle>& particles, 
                r64_t sigma, 
                r64_t epsilon, 
                r64_t boxLength, 
                r64_t rayCut, 
                r64_t skin, 
                idx_t stepEvery = 10);
            ~DosmLawLJPNB_OMP(void) = default;
            void kernel(Result* result) override;
    };

} // namespace dosm

#endif // DOSM_LAW_LJPNB_OMP_HPP