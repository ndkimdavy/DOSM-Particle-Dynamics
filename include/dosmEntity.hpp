#ifndef DOSM_ENTITY_HPP
#define DOSM_ENTITY_HPP

#include "dosmBasic.hpp"

namespace dosm
{
	class Entity
	{
		public:
			r64_t mass;
			r64_t charge;
			tensor_t<r64_t, 3> position;
			tensor_t<r64_t, 3> velocity;

			Entity(void) 
			{
				mass = 0.0;
				charge = 0.0;
				position(0) = position(1) = position(2) = 0.0;
				velocity(0) = velocity(1) = velocity(2) = 0.0;
			}

			explicit Entity(r64_t mass, r64_t charge = 0.0)
			{
				this->mass = mass;
				this->charge = charge;
				position(0) = position(1) = position(2) = 0.0;
				velocity(0) = velocity(1) = velocity(2) = 0.0;
			}

			virtual ~Entity(void) = default;
	};

} // namespace dosm

#endif // DOSM_ENTITY_HPP

