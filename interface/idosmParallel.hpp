#ifndef IDOSM_PARALLEL_HPP
#define IDOSM_PARALLEL_HPP

#include "dosmBasic.hpp"
#include <functional>

namespace dosm
{
    class IDosmParallel
    {
        public:
            virtual ~IDosmParallel(void) = default;

            virtual void init(void) = 0;
            virtual void release(void) = 0;

            virtual void dispatch(idx_t N, const std::function<void(idx_t)>& routine) = 0;
            virtual void dispatch(idx_t N, const std::function<void(idx_t, r64_t&)>& routine, r64_t& result) = 0;
    };

} // namespace dosm

#endif // IDOSM_PARALLEL_HPP
