//
// Created by ianpo on 16/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Collisions/RBContactResolver.hpp"


namespace Voxymore::Core
{
	RBContactResolver::RBContactResolver() : m_Iterations(0), m_PositionIterationsUsed(0) {}
	RBContactResolver::RBContactResolver(uint32_t iterations) : m_Iterations(iterations), m_PositionIterationsUsed(0) {}

	void RBContactResolver::ResolveContacts(TimeStep ts, std::vector<RigidbodyContact>& contacts)
	{
		VXM_PROFILE_FUNCTION();
		m_PositionIterationsUsed = 0;

		VXM_CORE_CHECK(m_Iterations > 0, "No iteration will be done as m_Iterations = {0}", m_Iterations);
		VXM_CORE_CHECK(m_Iterations >= contacts.size(), "There is not enough m_Iterations to cover all the m_Contacts... {0} m_Iterations for {1} m_Contacts", m_Iterations, contacts.size());

		if(contacts.empty()) {
			return;
		}

		PrepareContacts(ts, contacts);

		AdjustVelocities(ts, contacts);

		AdjustPositions(ts, contacts);
	}

	void RBContactResolver::SetIterations(uint32_t iterations)
	{
		this->m_Iterations = iterations;
	}

	void RBContactResolver::PrepareContacts(TimeStep ts, std::vector<RigidbodyContact>& contacts)
	{
		VXM_PROFILE_FUNCTION();
		for (RigidbodyContact& contact : contacts) {
			contact.CalculateInternals(ts);
		}
	}

	//TODO: used ordered list ? Or find some other optimization
	void RBContactResolver::AdjustPositions(TimeStep ts, std::vector<RigidbodyContact>& contacts)
	{
		VXM_PROFILE_FUNCTION();

		const size_t numContact = contacts.size();
		uint32_t i, index;
		std::array<Vec3, 2> linearChanges = {}, angularChanges = {};
		Real max;
		Vec3 deltaPosition;
		m_PositionIterationsUsed = 0;
		while (m_PositionIterationsUsed < m_Iterations)
		{
			max = m_ContactEpsilon;
			index = numContact;

			for (i = 0; i < numContact; ++i) {
				if(contacts[i] && contacts[i].penetration > max)
				{
					max = contacts[i].penetration;
					index = i;
				}
			}

			if(index == numContact)
			{
				break;
			}

			// contacts[index].MatchAwakeState();

			contacts[index].ApplyPositionChange(linearChanges, angularChanges, max);

			for (i = 0; i < numContact; ++i) {
				for (uint32_t b = 0; b < 2; ++b) {
					for (uint32_t d = 0; d < 2; ++d) {
						if (contacts[i].bodies[b] == contacts[index].bodies[d] && contacts[i].bodies[b] && contacts[index].bodies[d])
						{
							deltaPosition = linearChanges[d] + Math::Cross(angularChanges[d], contacts[i].m_RelativeContactPosition[b]);
							contacts[i].penetration += Math::Dot(deltaPosition, contacts[i].contactNormal) * (b ? (Real)1 : (Real)-1);
						}
					}
				}
			}
			m_PositionIterationsUsed++;
		}
	}

	void RBContactResolver::AdjustVelocities(TimeStep ts, std::vector<RigidbodyContact>& contacts)
	{
		VXM_PROFILE_FUNCTION();
		const size_t numContact = contacts.size();
		uint32_t i, index;
		std::array<Vec3, 2> linearChanges = {}, angularChanges = {};
		Real max;
		Vec3 deltaVelocity;
		m_VelocityIterationsUsed = 0;
		while (m_VelocityIterationsUsed < m_Iterations)
		{
			max = m_ContactEpsilon;
			index = numContact;

			for (i = 0; i < numContact; ++i) {
				if(contacts[i] && contacts[i].penetration > max)
				{
					max = contacts[i].penetration;
					index = i;
				}
			}

			if(index == numContact)
			{
				break;
			}

			// contacts[index].MatchAwakeState();

			contacts[index].ApplyVelocityChange(linearChanges, angularChanges);

			for (i = 0; i < numContact; ++i) {
				for (uint32_t b = 0; b < 2; ++b) {
					for (uint32_t d = 0; d < 2; ++d) {
						if (contacts[i].bodies[b] == contacts[index].bodies[d] && contacts[i].bodies[b] && contacts[index].bodies[d])
						{
							deltaVelocity = linearChanges[d] + Math::Cross(angularChanges[d], contacts[i].m_RelativeContactPosition[b]);

							// The sign of the change is negative if we're dealing
							// with the second body in a contact.
							contacts[i].m_ContactVelocity += contacts[i].GetWorldToContact() * deltaVelocity * (b ? (Real)-1 : (Real)1);
							contacts[i].CalculateDesiredDeltaVelocity(ts);
						}
					}
				}
			}
			m_VelocityIterationsUsed++;
		}
	}



} // namespace Voxymore::Core
