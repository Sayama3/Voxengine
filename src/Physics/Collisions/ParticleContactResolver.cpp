//
// Created by ianpo on 26/01/2024.
//

#include "Voxymore/Physics/Collisions/ParticleContactResolver.hpp"

namespace Voxymore::Core
{
	ParticleContactResolver::ParticleContactResolver(uint32_t iterations) : iterations(iterations), iterationsUsed(0)
	{}

	void ParticleContactResolver::ResolveContacts(TimeStep ts, std::vector<ParticleContact>& contacts)
	{
		uint32_t i;
		iterationsUsed = 0;

		VXM_CORE_CHECK(iterations >= contacts.size(), "There is not enough iterations to cover all the contacts... {0} iterations for {1} contacts", iterations, contacts.size());

		while (iterationsUsed < iterations)
		{
			Real max = REAL_MAX;
			uint32_t maxIndex = contacts.size();
			for (i = 0; i < contacts.size(); ++i)
			{
				Real sepVel = contacts[i].CalculateSeparatingVelocity();
				if(sepVel < max && (sepVel < 0 || contacts[0].penetration > 0))
				{
					max = sepVel;
					maxIndex = i;
				}
			}

			if(maxIndex == contacts.size())
			{
				break;
			}

			contacts[maxIndex].Resolve(ts);
			++iterationsUsed;
		}
	}

	void ParticleContactResolver::SetIterations(uint32_t iterations)
	{
	}
}// namespace Voxymore::Core