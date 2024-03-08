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
	};


	class CollisionDetector
	{
	public:
		static uint32_t SphereAndSphere(const Sphere& one, const Sphere& two, CollisionData* data);
		static uint32_t SphereAndHalfSpace(const Sphere &sphere, const Plane &plane, CollisionData *data);
		static uint32_t SphereAndTruePlane(const Sphere &sphere, const Plane &plane, CollisionData *data);
		static uint32_t BoxAndHalfSpace(const Box& box, const Plane &plane, CollisionData *data);
	};

} // namespace Voxymore::Core

