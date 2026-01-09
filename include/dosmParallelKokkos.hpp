#ifndef DOSM_PARALLEL_KOKKOS_HPP
#define DOSM_PARALLEL_KOKKOS_HPP

#include "idosmParallel.hpp"

namespace dosm
{
    class DosmParallelKokkos : public IDosmParallel
    {
        public:
            DosmParallelKokkos(void);
            virtual ~DosmParallelKokkos(void) = default;

            virtual void init(void) override;
            virtual void release(void) override;
            virtual void dispatch(idx_t N, const std::function<void(idx_t)>& routine) override;
            virtual void dispatch(idx_t N, const std::function<void(idx_t, r64_t&)>& routine, r64_t& result) override;
    };

} // namespace dosm

#endif // DOSM_PARALLEL_KOKKOS_HPP
