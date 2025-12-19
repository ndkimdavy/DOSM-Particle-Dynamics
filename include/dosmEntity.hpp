#ifndef DOSM_ENTITY_HPP
#define DOSM_ENTITY_HPP

#include "dosmBasic.hpp"
#include "dosmTensor.hpp"

namespace dosm
{
	class DosmEntity
	{
		public:
			r64_t mass;
			r64_t charge;
			tensor_t<r64_t, 3> position;
			tensor_t<r64_t, 3> velocity;

			DosmEntity(void);
			explicit DosmEntity(r64_t mass, r64_t charge);
			virtual ~DosmEntity(void) = default;
	};
}

#endif // DOSM_ENTITY_HPP

