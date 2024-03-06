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

		std::array<Rigidbody*, 2> bodies;

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

		Real friction;

		Real restitution;

		void SetBodyData(Rigidbody* one, Rigidbody* two, Real friction, Real restitution);
	};

	struct CollisionData
	{
		CollisionData();
		~CollisionData();
		int contactsLeft;
		Real friction;
		Real restitution;

		void AddContact(int i);
		RigidbodyContact* GetContact();
		RigidbodyContact* GetContact(int i);

		inline RigidbodyContact* begin()  { return contacts + (contactsLeft-1);}
		inline RigidbodyContact* end()  { return contacts + contactsCount;}
	private:
		RigidbodyContact* contacts;
		int contactsCount;
	};
} // namespace Voxymore::Core

