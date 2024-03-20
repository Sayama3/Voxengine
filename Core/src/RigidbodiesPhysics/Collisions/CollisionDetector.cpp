//
// Created by Sayama on 06/03/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/CollisionDetector.hpp"
#include <algorithm>


namespace Voxymore::Core
{
	bool IntersectionDetector::BoxAndHalfSpace(const Box& box, const Plane& plane)
	{
#if defined(__APPLE__) || defined(__MACH__)
		auto vertices = box.GetVertices();
		for(const Vec3& vertice : vertices) {
			if(Math::Dot(vertice, plane.m_Normal) <= plane.m_Offset) {
				return true;
			}
		}
		return false;
#else
		return std::ranges::any_of(box.GetVertices(), [&plane](Vec3& point) {
			return Math::Dot(point, plane.m_Normal) <= plane.m_Offset;
		});
#endif
	}
	bool IntersectionDetector::BoxAndBox(const Box &one, const Box &two)
	{
		Vec3 toCenter = two.GetPosition() - one.GetPosition();

		// All the axis to test using the SAT test.
		std::array<Vec3, 15> axis =
				{
						// Face axes for object one
						one.GetAxis(0),
						one.GetAxis(1),
						one.GetAxis(2),

						// Face axes for object two
						two.GetAxis(0),
						two.GetAxis(1),
						two.GetAxis(2),

						// Edge-Edge Cases
						Math::Cross(one.GetAxis(0), two.GetAxis(0)),
						Math::Cross(one.GetAxis(0), two.GetAxis(1)),
						Math::Cross(one.GetAxis(0), two.GetAxis(2)),

						Math::Cross(one.GetAxis(1), two.GetAxis(0)),
						Math::Cross(one.GetAxis(1), two.GetAxis(1)),
						Math::Cross(one.GetAxis(1), two.GetAxis(2)),

						Math::Cross(one.GetAxis(2), two.GetAxis(0)),
						Math::Cross(one.GetAxis(2), two.GetAxis(1)),
						Math::Cross(one.GetAxis(2), two.GetAxis(2)),
				};

		return std::all_of(axis.begin(), axis.end(), [&one, &two, toCenter](Vec3 axe){
			if(Math::SqrMagnitude(axe) < 0.001) return true;
			axe = Math::Normalize(axe);
			return IntersectionDetector::PenetrationOnAxis(one, two, axe, toCenter) >= 0;
		});
	}

	Real IntersectionDetector::TransformToAxis(const Box& box, const Vec3& axis)
	{
		return box.m_HalfSize.x * Math::Abs(Math::Dot(axis, Vec3(box.GetAxis(0)))) +
			   box.m_HalfSize.y * Math::Abs(Math::Dot(axis, Vec3(box.GetAxis(1)))) +
			   box.m_HalfSize.z * Math::Abs(Math::Dot(axis, Vec3(box.GetAxis(2))));
	}

	Real IntersectionDetector::PenetrationOnAxis(const Voxymore::Core::Box &one, const Voxymore::Core::Box &two, const Voxymore::Core::Vec3 &axis, const Voxymore::Core::Vec3 &toCenter){
		Real oneProject = TransformToAxis(one, axis);
		Real twoProject = TransformToAxis(two, axis);

		Real distance = Math::Abs(Math::Dot(toCenter, axis));

		// Return the overlap (i.e. positive indicates overlap, negative indicates separation).
		return oneProject + twoProject - distance;
	}

	uint32_t CollisionDetector::SphereAndSphere(const Sphere &one, const Sphere &two, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data)
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

		RigidbodyContact contacts;
		contacts.contactNormal = normal;
		contacts.contactPoint = pOne + midline * (Real)0.5;
		contacts.penetration = (one.m_Radius + two.m_Radius - size);
		contacts.SetBodyData(one.m_Body, two.m_Body, data->friction, data->restitution);
		data->AddContact(contacts);

		return 1;
	}

	uint32_t CollisionDetector::SphereAndHalfSpace(const Sphere &sphere, const Plane &plane, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data)
		{
			return 0;
		}

		Vec3 position = sphere.GetPosition();
		Real ballDistance = Math::Dot(plane.m_Normal, position) - sphere.m_Radius - plane.m_Offset;

		if (ballDistance >= 0) return 0;

		RigidbodyContact contact;

		contact.contactNormal = plane.m_Normal;
		contact.penetration = -ballDistance;
		contact.contactPoint = position - plane.m_Normal * (ballDistance + sphere.m_Radius);
		contact.SetBodyData(sphere.m_Body, nullptr, data->friction, data->restitution);
		data->AddContact(contact);

		return 1;
	}

	uint32_t CollisionDetector::SphereAndTruePlane(const Sphere &sphere, const Plane &plane, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data)
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

		RigidbodyContact contact;
		contact.contactPoint = position - normal * centerDistance;
		contact.contactNormal = normal;
		contact.penetration = penetration;
		contact.SetBodyData(sphere.m_Body, nullptr,data->friction ,data->restitution);
		data->AddContact(contact);

		return 1;
	}

	uint32_t CollisionDetector::BoxAndHalfSpace(const Box& box, const Plane& plane, CollisionData* data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data)
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
				RigidbodyContact contact;
				contact.contactPoint = plane.m_Normal;
				contact.contactPoint *= (vertDistance -plane.m_Offset);
				contact.contactPoint += vertexPos;
				contact.contactNormal = plane.m_Normal;
				contact.penetration = plane.m_Offset - vertDistance;
				contact.SetBodyData(box.m_Body, nullptr, data->friction, data->restitution);
				data->AddContact(contact);
				contactUsed++;
			}
		}

		return contactUsed;
	}

	uint32_t CollisionDetector::BoxAndSphere(const Box &box, const Sphere &sphere, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data)
		{
			return 0;
		}

		Vec3 sphereCenter = sphere.GetPosition();
		Vec3 relativeCenter = Math::TransformPoint(Math::Inverse(box.GetMatrix()), sphereCenter);

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

		Vec3 closestPtWorld = Math::TransformPoint(box.GetMatrix(), closest);

		RigidbodyContact contact;
		contact.contactPoint = closestPtWorld;
		contact.contactNormal = Math::Normalize(closestPtWorld - sphereCenter);
		contact.penetration = sphere.m_Radius - Math::Sqrt(dist);
		contact.SetBodyData(box.m_Body, sphere.m_Body, data->friction, data->restitution);
		data->AddContact(contact);

		return 1;
	}

	Vec3 GetContactPoint(const Vec3& axisOne, const Vec3& axisTwo, const Vec3& ptOnEdgeOne, const Vec3& ptOnEdgeTwo)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 toSt = ptOnEdgeOne - ptOnEdgeTwo;

		Real dpStaOne = Math::Dot(axisOne, toSt);
		Real dpStaTwo = Math::Dot(axisTwo, toSt);

		Real smOne = Math::SqrMagnitude(axisOne);
		Real smTwo = Math::SqrMagnitude(axisTwo);

		Real dotProductEdges = Math::Dot(axisTwo, axisOne);
		Real denom = smOne * smTwo - dotProductEdges * dotProductEdges;

		Real a = (dotProductEdges * dpStaTwo - smTwo * dpStaOne) / denom;
		Real b = (smOne * dpStaTwo - dotProductEdges * dpStaOne) / denom;

		Vec3 nearestPtOnOne = ptOnEdgeOne + axisOne * a;
		Vec3 nearestPtOnTwo = ptOnEdgeTwo + axisTwo * b;

		return nearestPtOnOne * (Real)0.5 + nearestPtOnTwo * (Real)0.5;
	}

	uint32_t CollisionDetector::BoxAndBox(const Box &one, const Box &two, CollisionData *data)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(data, "The CollisionData is not valid.");
		if(!data)
		{
			return 0;
		}

		Real bestOverlap = REAL_MAX;
		int32_t bestCase = -1;

		Vec3 toCenter = two.GetPosition() - one.GetPosition();

		// All the axis to test using the SAT test.
		std::array<Vec3, 15> axis =
		{
						// Face axes for object one
						one.GetAxis(0),
						one.GetAxis(1),
						one.GetAxis(2),

						// Face axes for object two
						two.GetAxis(0),
						two.GetAxis(1),
						two.GetAxis(2),

						// Edge-Edge Cases
						Math::Cross(one.GetAxis(0), two.GetAxis(0)),
						Math::Cross(one.GetAxis(0), two.GetAxis(1)),
						Math::Cross(one.GetAxis(0), two.GetAxis(2)),

						Math::Cross(one.GetAxis(1), two.GetAxis(0)),
						Math::Cross(one.GetAxis(1), two.GetAxis(1)),
						Math::Cross(one.GetAxis(1), two.GetAxis(2)),

						Math::Cross(one.GetAxis(2), two.GetAxis(0)),
						Math::Cross(one.GetAxis(2), two.GetAxis(1)),
						Math::Cross(one.GetAxis(2), two.GetAxis(2)),
		};

		// Finding the best scenario (if any)
		for (int32_t i = 0; i < 15; ++i)
		{
			auto& axe = axis[i];
			if(Math::SqrMagnitude(axe) < 0.001) continue;

			axe = Math::Normalize(axe);

			Real overlap = IntersectionDetector::PenetrationOnAxis(one, two, axe, toCenter);
			if (overlap < 0) {
				return 0;
			}
			else if (overlap < bestOverlap) {
				bestOverlap = overlap;
				bestCase = i;
			}
		}

		VXM_CORE_ASSERT(bestCase >= 0, "No best case found...");
		if(bestCase < 0) return 0;

		if(bestCase < 3) // Face Object one
		{
			RigidbodyContact contact;

			Vec3 normal = one.GetAxis(bestCase);
			if (Math::Dot(one.GetAxis(bestCase), toCenter) > 0)
			{
				normal *= (Real)-1.0;
			}

			// Work out which vertex of box two we're colliding with.
			Vec3 vertex = two.m_HalfSize;
			if (Math::Dot(two.GetAxis(0), normal) < 0) vertex.x = -vertex.x;
			if (Math::Dot(two.GetAxis(1), normal) < 0) vertex.y = -vertex.y;
			if (Math::Dot(two.GetAxis(2), normal) < 0) vertex.z = -vertex.z;

			// Create the contact data
			contact.contactNormal = normal;
			contact.penetration = bestOverlap;
			contact.contactPoint = Math::TransformPoint(two.GetMatrix(), vertex);
			contact.SetBodyData(one.m_Body, two.m_Body, data->friction, data->restitution);
			data->AddContact(contact);
			return 1;
		}
		else if(bestCase < 6) // Face Object two
		{
			RigidbodyContact contact;

			Vec3 normal = two.GetAxis(bestCase-3);
			if (Math::Dot(two.GetAxis(bestCase-3), (toCenter * (Real)-1)) > 0)
			{
				normal *= (Real)-1.0;
			}

			// Work out which vertex of box two we're colliding with.
			Vec3 vertex = one.m_HalfSize;
			if (Math::Dot(one.GetAxis(0), normal) < 0) vertex.x = -vertex.x;
			if (Math::Dot(one.GetAxis(1), normal) < 0) vertex.y = -vertex.y;
			if (Math::Dot(one.GetAxis(2), normal) < 0) vertex.z = -vertex.z;

			// Create the contact data
			contact.contactNormal = normal;
			contact.penetration = bestOverlap;
			contact.contactPoint = Math::TransformPoint(one.GetMatrix(), vertex);
			contact.SetBodyData(two.m_Body, one.m_Body, data->friction, data->restitution);
			data->AddContact(contact);
			return 1;
		}
		else // Edge vs Edge contact
		{
			// We've got an edge-edge contact. Find out which axes
			bestCase -= 6;
			int32_t oneAxisIndex = bestCase / 3;
			int32_t twoAxisIndex = bestCase % 3;

			Vec3 oneAxis = one.GetAxis(oneAxisIndex);
			Vec3 twoAxis = two.GetAxis(twoAxisIndex);
			Vec3 axe = Math::Normalize(Math::Cross(oneAxis, twoAxis));
			// The axis should point from box one to box two.
			if (Math::Dot(axe, toCenter) > 0) axe *= (Real) -1;

			auto ptOnEdgeOne = one.m_HalfSize;
			auto ptOnEdgeTwo = two.m_HalfSize;

			for (int32_t i = 0; i < 3; ++i) {
				if(i == oneAxisIndex) ptOnEdgeOne[i] = 0;
				else if(Math::Dot(one.GetAxis(i), axe) > 0) ptOnEdgeOne[i] *= (Real)-1.0;

				if(i == twoAxisIndex) ptOnEdgeTwo[i] = 0;
				else if(Math::Dot(two.GetAxis(i), axe) > 0) ptOnEdgeTwo[i] *= (Real)-1.0;
			}

			RigidbodyContact contact;
			contact.contactPoint = GetContactPoint(oneAxis, twoAxis, ptOnEdgeOne, ptOnEdgeTwo);
			contact.contactNormal = axe;
			contact.penetration = bestOverlap;
			contact.SetBodyData(two.m_Body, one.m_Body, data->friction, data->restitution);
			data->AddContact(contact);
			return 1;
		}
	}
} // namespace Voxymore::Core
