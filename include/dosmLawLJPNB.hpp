#ifndef DOSM_LAW_LJPNB_HPP
#define DOSM_LAW_LJPNB_HPP

#include "dosmLawLJP.hpp"

namespace dosm
{
    class DosmLawLJPNB : public DosmLawLJP
    {
        public:
            DosmLawLJPNB(
                vector_t<DosmParticle>& particles, 
                r64_t sigma, 
                r64_t epsilon, 
                r64_t boxLength, 
                r64_t rayCut, 
                r64_t skin, 
                idx_t stepEvery = 10);
            ~DosmLawLJPNB(void) = default;
            void kernel(Result* result) override;

        protected:
            vector_t<vector_t<idx_t>> neighbor;

        private:
            r64_t skin;
            idx_t stepEvery;
            idx_t stepCount = 0;
    };

} // namespace dosm

#endif // DOSM_LAW_LJPNB_HPP

