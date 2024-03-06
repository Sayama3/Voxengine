//
// Created by Sayama on 06/03/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/CollisionDetector.hpp"

namespace Voxymore
{
	namespace Core
	{
		uint32_t CollisionDetector::SphereAndSphere(const Sphere &one, const Sphere &two, CollisionData *data)
		{
			VXM_PROFILE_FUNCTION();
			VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
			if(!data || data->contactsLeft <= 0)
			{
				return 0;
			}

			Vec3 pOne = one.GetPosition();
			Vec3 pTwo = two.GetPosition();

			Vec3 midline = pOne - pTwo;
			Real size = Math::Magnitude(midline);

			if(size <= 0.0f || size >= one.m_Radius + two.m_Radius)
			{
				return 0;
			}

			Vec3 normal = midline * (Real)1.0 / size;

			auto* contacts = data->GetContact();
			contacts->contactNormal = normal;
			contacts->contactPoint = pOne + midline * (Real)0.5;
			contacts->penetration = (one.m_Radius + two.m_Radius - size);
			contacts->SetBodyData(one.m_Body, two.m_Body, data->friction, data->restitution);
			data->AddContact(1);
		}
	}// namespace Core
}// namespace Voxymore