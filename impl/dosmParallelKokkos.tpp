#include <Kokkos_Core.hpp>

namespace dosm {

	template<typename functor>
		void DosmParallel<functor>::init(void) 
		{ 
			Kokkos::initialize();
		}

	template<typename functor>
		void DosmParallel<functor>::release(void) 
		{
			Kokkos::finalize();
		}

	template<typename functor>
		void DosmParallel<functor>::dispatch(idx_t N, functor routine)
		{
			Kokkos::parallel_for(
					Kokkos::RangePolicy<>(0, static_cast<idx_t>(N)), 
					routine
					);
		}

	template<typename functor>
		void DosmParallel<functor>::dispatch(idx_t N, functor routine, r64_t& result)
		{
			r64_t tmp = 0.0;
			Kokkos::parallel_reduce(
					Kokkos::RangePolicy<>(0, static_cast<idx_t>(N)),
					routine,
					tmp
					);
			result = tmp;
		}

} // namespace dosm
