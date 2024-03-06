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

	class CollisionDetector
	{
	public:
		static uint32_t SphereAndSphere(const Sphere& one, const Sphere& two, CollisionData* data);
	};

} // namespace Voxymore::Core

