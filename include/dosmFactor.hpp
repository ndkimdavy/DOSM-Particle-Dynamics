#ifndef DOSM_FACTOR_HPP
#define DOSM_FACTOR_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"
#include "idosmLaw.hpp"
#include "dosmParallel.hpp"
#include <memory>
#include <functional>

namespace dosm
{
    class DosmFactor
    {
        public:
            DosmFactor(const str_t& file);
            ~DosmFactor(void) = default;
            void run(void); 

        private:
            void loadFile(const str_t& file);
            void outFile(void);

        private:
            DosmParticleSnap dosmParticleSnap;
            std::unique_ptr<IDosmLaw> idosmLaw;
            DosmParallel<std::function<void(idx_t)>> dosmParallel;
    };

} // namespace dosm

#endif // DOSM_FACTOR_HPP

