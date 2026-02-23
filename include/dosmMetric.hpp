#ifndef DOSM_METRIC_HPP
#define DOSM_METRIC_HPP

#include "dosmBasic.hpp"
#include "dosmParticle.hpp"
#include "idosmLaw.hpp"
#include "idosmSocket.hpp"
#include <memory>
#include <functional>

namespace dosm
{
    class DosmMetric
    {
        public:
            DosmMetric(void);
            ~DosmMetric(void) = default;
            void run(void); 

        private:
            void loadFile(const str_t& file);
            void loadFileExam(const str_t& xyzFile, const str_t& mciFile);
            void outFile(void);

        private:
            DosmParticleSnap dosmParticleSnap;
            std::unique_ptr<IDosmLaw> baseLaw;
            std::unique_ptr<IDosmLaw> idosmLaw;
            std::unique_ptr<IDosmSocket> idosmSocket;
    };

} // namespace dosm

#endif // DOSM_METRIC_HPP

