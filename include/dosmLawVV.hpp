#ifndef DOSM_LAW_VV_HPP
#define DOSM_LAW_VV_HPP

#include "idosmLaw.hpp"

namespace dosm
{
    class DosmLawVV : public IDosmLaw
    {
        public:
            DosmLawVV(IDosmLaw& idosmLaw, 
                DosmParticleSnap::Snap& snap, 
                r64_t dt, 
                r64_t boxLength);
            ~DosmLawVV(void) = default;
            void kernel(Result* result) override;

        private:
            void init(void);

        protected:
            IDosmLaw& idosmLaw;
            DosmParticleSnap::Snap& snap;
            r64_t dt;
            r64_t boxLength;
            idx_t stepCount = 0;
    };

} // namespace dosm

#endif // DOSM_LAW_VV_HPP
