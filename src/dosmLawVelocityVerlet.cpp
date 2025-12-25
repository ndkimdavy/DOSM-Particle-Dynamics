#include "dosmLawVelocityVerlet.hpp"

namespace dosm
{
	DosmLawVelocityVerlet::DosmLawVelocityVerlet(IDosmLaw& idosmLaw, DosmParticleSnap::Snap& snap, r64_t dt):
		idosmLaw(idosmLaw), 
		snap(snap)
	{
		this->dt = dt;
	}

	void DosmLawVelocityVerlet::kernel(Result* result)
	{
		idx_t n = snap.particles.size();

		// F(t)
		idosmLaw.kernel(result);

		// v(t+1/2), r(t+dt)
		for (idx_t i = 0; i < n; ++i)
		{
			DosmParticle& p = snap.particles[i];
			tensor_t<r64_t, 3> velocity_half = p.velocity + 0.5 * (p.force * (1.0 / p.mass)) * dt;
			p.position = p.position + velocity_half * dt;
			p.velocity = velocity_half;
		}

		// F(t+dt)
		idosmLaw.kernel(result);

		// v(t+dt)
		for (idx_t i = 0; i < n; ++i)
		{
			DosmParticle& p = snap.particles[i];
			p.velocity = p.velocity + 0.5 * (p.force * (1.0 / p.mass)) * dt;
		}

		snap.t += dt;
	}

} // namespace dosm
