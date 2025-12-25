#ifndef DOSM_LAW_VELOCITY_VERLET_HPP
#define DOSM_LAW_VELOCITY_VERLET_HPP

#include "idosmLaw.hpp"

namespace dosm
{
	class DosmLawVelocityVerlet : public IDosmLaw
	{
		public:
			DosmLawVelocityVerlet(IDosmLaw& idosmLaw, DosmParticleSnap::Snap& snap, r64_t dt);
			~DosmLawVelocityVerlet(void) = default;

			void kernel(Result* result) override;

		protected:
			IDosmLaw& idosmLaw;
			DosmParticleSnap::Snap& snap;
			r64_t dt;
	};

} // namespace dosm

#endif // DOSM_LAW_VELOCITY_VERLET_HPP
