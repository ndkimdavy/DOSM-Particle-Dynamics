#ifndef DOSM_LAW_LJ_HPP
#define DOSM_LAW_LJ_HPP

#include "idosmLaw.hpp"

namespace dosm
{
    class DosmLawLJ : public IDosmLaw
    {
        public:
            DosmLawLJ(vector_t<DosmParticle>& particles, r64_t sigma, r64_t epsilon);
            ~DosmLawLJ(void) = default;
            void kernel(Result* result) override;

        protected:
            vector_t<DosmParticle>& particles;
            r64_t sigma;
            r64_t epsilon;
    };

} // namespace dosm

#endif // DOSM_LAW_LJ_HPP

