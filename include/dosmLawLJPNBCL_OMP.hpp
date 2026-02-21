#ifndef DOSM_LAW_LJPNBCL_OMP_HPP
#define DOSM_LAW_LJPNBCL_OMP_HPP

#include "dosmLawLJPNB.hpp"

namespace dosm
{
    class DosmLawLJPNBCL_OMP : public DosmLawLJPNB
    {
        public:
            DosmLawLJPNBCL_OMP(
                vector_t<DosmParticle>& particles,
                r64_t sigma,
                r64_t epsilon,
                r64_t boxLength,
                r64_t rayCut,
                r64_t skin,
                idx_t dimX,
                idx_t dimY,
                idx_t stepEvery = 10);
            ~DosmLawLJPNBCL_OMP(void) override = default;
            void kernel(Result* result) override;

        protected:
            idx_t dimX;
            idx_t dimY;
            r64_t subBoxLengthX;
            r64_t subBoxLengthY;
            vector_t<vector_t<idx_t>> grid;
            void buildGrid(void);
    };

} // namespace dosm

#endif // DOSM_LAW_LJPNBCL_OMP_HPP