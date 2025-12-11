#include "dosmParallelKokkos.hpp"

void DosmParallelKokkos::init() 
{ 
	Kokkos::initialize();
}

void DosmParallelKokkos::release() 
{
	Kokkos::finalize();
}

void DosmParallelKokkos::dispatch(dosm::idx_t N, const std::function<void(dosm::idx_t)>& routine)
{
	Kokkos::parallel_for(
		Kokkos::RangePolicy<>(0, static_cast<int>(N)), 
		KOKKOS_LAMBDA(const int i) { routine(static_cast<dosm::idx_t>(i)); }
	);
}

void DosmParallelKokkos::dispatch(dosm::idx_t N, const std::function<dosm::r64_t(dosm::idx_t)>& routine, dosm::r64_t& result)
{
	dosm::r64_t tmp = 0.0;
	Kokkos::parallel_reduce(
		Kokkos::RangePolicy<>(0, static_cast<int>(N)), 
		KOKKOS_LAMBDA(const int i, dosm::r64_t& local) { local += routine(static_cast<dosm::idx_t>(i)); }, 
		tmp
	);
	result = tmp;
}

