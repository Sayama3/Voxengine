//
// Created by ianpo on 16/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	class RigidbodyContact
	{
	public:
		inline RigidbodyContact() = default;
		inline ~RigidbodyContact() = default;

		/**
		 * Holds the position of the contact in world coordinates.
		 */
		Vec3 contactPoint;

		/**
		 * The direction of the contact in world space.
		 */
		Vec3 contactNormal;

		/**
		 * The depth of penetration at the contact.
		 */
		Real penetration;
	};
} // namespace Voxymore::Core

