#ifndef DOSM_BASIC_HPP
#define DOSM_BASIC_HPP

#include "dosmLog.hpp"
#include <cstdint>
#include <complex>
#include <cstddef>

namespace dosm {

	using i8_t   = std::int8_t;
	using i16_t  = std::int16_t;
	using i32_t  = std::int32_t;
	using i64_t  = std::int64_t;

	using ui8_t  = std::uint8_t;
	using ui16_t = std::uint16_t;
	using ui32_t = std::uint32_t;
	using ui64_t = std::uint64_t;

	using r32_t = float;
	using r64_t = double;

	using c32_t = std::complex<float>;
	using c64_t = std::complex<double>;

	using idx_t  = std::size_t;
	using buf_t  = std::uint8_t;
	using bool_t = bool;

} // namespace dosm

#endif // DOSM_BASIC_HPP
















