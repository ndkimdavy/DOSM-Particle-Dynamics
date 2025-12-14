#ifndef DOSM_PARALLEL_HPP
#define DOSM_PARALLEL_HPP

#include "dosmBasic.hpp"

namespace dosm {

	template<typename functor>
		class DosmParallel {
			public:
				void init(void);
				void release(void);
				void dispatch(idx_t N, functor routine);
				void dispatch(idx_t N, functor routine, r64_t& result);
		};

} // namespace dosm

#ifdef DOSM_PARALLEL_KOKKOS
#include "dosmParallelKokkos.tpp"
#endif

#endif // DOSM_PARALLEL_HPP

