//
// Created by ianpo on 16/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContactResolver.hpp"


namespace Voxymore::Core
{
	/*
	RigidbodyContactResolver::RigidbodyContactResolver() : iterations(0), iterationsUsed(0) {}
	RigidbodyContactResolver::RigidbodyContactResolver(uint32_t iterations) : iterations(iterations), iterationsUsed(0) {}

	void RigidbodyContactResolver::ResolveContacts(TimeStep ts, std::vector<RigidbodyContact>& contacts)
	{
		uint32_t i;
		iterationsUsed = 0;

		VXM_CORE_CHECK(iterations <= 0, "No iteration will be done as iterations = {0}", iterations);
		VXM_CORE_CHECK(iterations >= contacts.size(), "There is not enough iterations to cover all the m_Contacts... {0} iterations for {1} m_Contacts", iterations, contacts.size());

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

	void RigidbodyContactResolver::SetIterations(uint32_t iterations)
	{
		this->iterations = iterations;
	}
*/
} // namespace Voxymore::Core
