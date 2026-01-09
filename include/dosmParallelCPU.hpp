#ifndef DOSM_PARALLEL_CPU_HPP
#define DOSM_PARALLEL_CPU_HPP

#include "idosmParallel.hpp"

namespace dosm
{
    class DosmParallelCPU : public IDosmParallel
    {
        public:
            DosmParallelCPU(idx_t tids);
            ~DosmParallelCPU(void) = default;

            void init(void) override;
            void release(void) override;
            void dispatch(idx_t N, const std::function<void(idx_t)>& routine) override;
            void dispatch(idx_t N, const std::function<void(idx_t, r64_t&)>& routine, r64_t& result) override;

        private:
            idx_t tids;
    };

} // namespace dosm

#endif // DOSM_PARALLEL_CPU_HPP
