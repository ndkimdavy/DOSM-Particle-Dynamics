#ifndef DOSM_PARALLEL_KOKKOS_HPP
#define DOSM_PARALLEL_KOKKOS_HPP

#include "idosmParallel.hpp"
#include <Kokkos_Core.hpp>

class DosmParallelKokkos : public IDosmParallel {
	public:
		DosmParallelKokkos(void) = default;
		~DosmParallelKokkos(void) override = default;
		void init(void) override;
		void release(void) override;
		void dispatch(dosm::idx_t N, const std::function<void(dosm::idx_t)>& routine) override;
		void dispatch(dosm::idx_t N, const std::function<dosm::r64_t(dosm::idx_t)>& routine, dosm::r64_t& result) override;
};

#endif // DOSM_PARALLEL_KOKKOS_HPP

