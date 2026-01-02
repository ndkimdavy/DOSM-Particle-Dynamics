#ifndef DOSM_LAW_VELOCITY_VERLET_HPP
#define DOSM_LAW_VELOCITY_VERLET_HPP

#include "idosmLaw.hpp"

namespace dosm
{
	class DosmLawVelocityVerlet : public IDosmLaw
	{
		public:
			DosmLawVelocityVerlet(IDosmLaw& idosmLaw, DosmParticleSnap::Snap& snap, r64_t dt, r64_t boxLength);
			~DosmLawVelocityVerlet(void) = default;
			void kernel(Result* result) override;

		private:
			void init(void);

		protected:
			IDosmLaw& idosmLaw;
			DosmParticleSnap::Snap& snap;
			r64_t dt;
			r64_t boxLength;
	};

} // namespace dosm

#endif // DOSM_LAW_VELOCITY_VERLET_HPP
