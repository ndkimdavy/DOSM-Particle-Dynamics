#ifndef IDOSM_PARALLEL_HPP
#define IDOSM_PARALLEL_HPP

#include "dosmBasic.hpp"
#include <functional>

class IDosmParallel {
	public:
		virtual ~IDosmParallel() = default;
		virtual void init() = 0;
		virtual void release() = 0;
		virtual void dispatch(dosm::idx_t N, const std::function<void(dosm::idx_t)>& routine) = 0;
		virtual void dispatch(dosm::idx_t N, const std::function<dosm::r64_t(dosm::idx_t)>& routine, dosm::r64_t& result) = 0;
};

#endif // IDOSM_PARALLEL_HPP

