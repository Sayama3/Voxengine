//
// Created by Sayama on 06/03/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/CollisionDetector.hpp"


namespace Voxymore::Core
{
	bool IntersectionDetector::BoxAndHalfSpace(const Box& box, const Plane& plane)
	{
		return std::ranges::any_of(box.GetVertices(), [&plane](Vec3& point) {
			return Math::Dot(point, plane.m_Normal) <= plane.m_Offset;
		});
	}

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
		data->AddContact();

		return 1;
	}

	uint32_t CollisionDetector::SphereAndHalfSpace(const Sphere &sphere, const Plane &plane, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data || data->contactsLeft <= 0)
		{
			return 0;
		}

		Vec3 position = sphere.GetPosition();
		Real ballDistance = Math::Dot(plane.m_Normal, position) - sphere.m_Radius - plane.m_Offset;

		if (ballDistance >= 0) return 0;

		RigidbodyContact* contact = data->GetContact();

		contact->contactNormal = plane.m_Normal;
		contact->penetration = -ballDistance;
		contact->contactPoint = position - plane.m_Normal * (ballDistance + sphere.m_Radius);
		contact->SetBodyData(sphere.m_Body, nullptr, data->friction, data->restitution);
		data->AddContact();

		return 1;
	}

	uint32_t CollisionDetector::SphereAndTruePlane(const Sphere &sphere, const Plane &plane, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data || data->contactsLeft <= 0)
		{
			return 0;
		}

		Vec3 position = sphere.GetPosition();
		Real centerDistance = Math::Dot(plane.m_Normal, position) - plane.m_Offset;

		if(centerDistance * centerDistance > sphere.m_Radius * sphere.m_Radius)
		{
			return 0;
		}

		Vec3 normal = plane.m_Normal;
		Real penetration = -centerDistance;
		if(centerDistance < 0)
		{
			normal = -normal;
			penetration = -penetration;
		}
		penetration += sphere.m_Radius;

		RigidbodyContact* contact = data->GetContact();
		contact->contactPoint = position - normal * centerDistance;
		contact->contactNormal = normal;
		contact->penetration = penetration;

		contact->SetBodyData(sphere.m_Body, nullptr,data->friction ,data->restitution);
		data->AddContact();

		return 1;
	}

	uint32_t CollisionDetector::BoxAndHalfSpace(const Box& box, const Plane& plane, CollisionData* data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data || data->contactsLeft <= 0)
		{
			return 0;
		}

		if(!IntersectionDetector::BoxAndHalfSpace(box, plane))
		{
			return 0;
		}

		auto vertices = box.GetVertices();
		int contactUsed = 0;

		for(auto& vertexPos: vertices)
		{
			Real vertDistance = Math::Dot(vertexPos, plane.m_Normal);

			if(vertDistance <= plane.m_Offset)
			{
				auto* contact = data->GetContact();
				contact->contactPoint = plane.m_Normal;
				contact->contactPoint *= (vertDistance -plane.m_Offset);
				contact->contactPoint += vertexPos;
				contact->contactPoint = plane.m_Normal;
				contact->penetration = plane.m_Offset - vertDistance;
				contact->SetBodyData(box.m_Body, nullptr, data->friction, data->restitution);
				data->AddContact();
				contactUsed++;
				if(data->contactsLeft <= 0) break;
			}
		}

		return contactUsed;
	}

	uint32_t CollisionDetector::BoxAndSphere(const Box &box, const Sphere &sphere, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data || data->contactsLeft <= 0)
		{
			return 0;
		}

		Vec3 sphereCenter = sphere.GetPosition();
		Vec3 relativeCenter = Math::TransformPoint(Math::Inverse(box.m_Offset), sphereCenter);

		if (Math::Abs(relativeCenter.x) - sphere.m_Radius > box.m_HalfSize.x ||
			Math::Abs(relativeCenter.y) - sphere.m_Radius > box.m_HalfSize.y ||
			Math::Abs(relativeCenter.z) - sphere.m_Radius > box.m_HalfSize.z)
		{
			return 0;
		}

		Vec3 closest = {0,0,0};
		Real dist;

		dist = relativeCenter.x;
		if(dist > box.m_HalfSize.x) dist = box.m_HalfSize.x;
		if(dist < -box.m_HalfSize.x) dist = -box.m_HalfSize.x;
		closest.x = dist;

		dist = relativeCenter.y;
		if(dist > box.m_HalfSize.y) dist = box.m_HalfSize.y;
		if(dist < -box.m_HalfSize.y) dist = -box.m_HalfSize.y;
		closest.y = dist;

		dist = relativeCenter.z;
		if(dist > box.m_HalfSize.z) dist = box.m_HalfSize.z;
		if(dist < -box.m_HalfSize.z) dist = -box.m_HalfSize.z;
		closest.z = dist;

		dist = Math::SqrMagnitude(closest - relativeCenter);
		if(dist > Math::Pow2(sphere.m_Radius)) return 0;

		Vec3 closestPtWorld = Math::TransformPoint(box.m_Offset, closest);

		auto* contact = data->GetContact();
		contact->contactPoint = closestPtWorld;
		contact->contactNormal = Math::Normalize(closestPtWorld - sphereCenter);
		contact->penetration = sphere.m_Radius - Math::Sqrt(dist);
		contact->SetBodyData(box.m_Body, sphere.m_Body, data->friction, data->restitution);
		data->AddContact();

		return 1;
	}
} // namespace Voxymore::Core
