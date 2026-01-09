#ifndef DOSM_LAW_LJP_HPP
#define DOSM_LAW_LJP_HPP

#include "dosmLawLJ.hpp"

namespace dosm
{
    class DosmLawLJP : public DosmLawLJ
    {
        public:
            DosmLawLJP(vector_t<DosmParticle>& particles, r64_t sigma, r64_t epsilon, r64_t boxLength, r64_t rayCut);
            ~DosmLawLJP(void) = default;
            void kernel(Result* result) override;

        protected:
            r64_t boxLength;
            r64_t rayCut;
            vector_t<tensor_t<r64_t, 3>> images;
    };

} // namespace dosm

#endif // DOSM_LAW_LJP_HPP

