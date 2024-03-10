//
// Created by Sayama on 06/03/2024.
//

#pragma once

#include "Voxymore/Math/Math.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"
#include "Voxymore/RigidbodiesPhysics/Primitive.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/RigidbodyContact.hpp"

namespace Voxymore::Core
{
	class IntersectionDetector
	{
	public:
		static bool BoxAndHalfSpace(const Box& box, const Plane &plane);
		/**
		 * Calculates the penetration between two boxes along a given axis.
		 *
		 * This static member function calculates the penetration between two boxes, `one` and `two`, along a given axis vector `axis`.
		 * The `toCenter` parameter represents the displacement vector from the center of `one` to the center of `two`.
		 *
		 * @param one The first box.
		 * @param two The second box.
		 * @param axis The axis along which to calculate the penetration.
		 * @param toCenter The displacement vector from the center of `one` to the center of `two`.
		 * @return The penetration distance between the two boxes along the given axis. (i.e. positive indicates overlap, negative indicates separation)
		 *
		 * @see CollisionDetector
		 * @see CollisionDetector::TransformToAxis()
		 */
		static Real PenetrationOnAxis(const Box& one, const Box& two, const Vec3& axis, const Vec3& toCenter);
		static bool BoxAndBox(const Box& one, const Box& two);
		static Real TransformToAxis(const Box& box, const Vec3& axis);
	};


	class CollisionDetector
	{
	public:
		static uint32_t SphereAndSphere(const Sphere& one, const Sphere& two, CollisionData* data);
		static uint32_t SphereAndHalfSpace(const Sphere &sphere, const Plane &plane, CollisionData *data);
		static uint32_t SphereAndTruePlane(const Sphere &sphere, const Plane &plane, CollisionData *data);
		static uint32_t BoxAndHalfSpace(const Box& box, const Plane &plane, CollisionData *data);
		static uint32_t BoxAndSphere(const Box& box, const Sphere& sphere, CollisionData* data);

		static uint32_t BoxAndBox(const Box &one, const Box &two, CollisionData *data);
	};

} // namespace Voxymore::Core

