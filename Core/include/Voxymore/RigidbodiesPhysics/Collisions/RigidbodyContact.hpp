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
		Real friction;
		Real restitution;

		void AddContact(int i = 1);
		RigidbodyContact* GetContact();
		RigidbodyContact* GetContact(int i);

		inline std::vector<RigidbodyContact>::iterator begin()  { return contacts.begin();}
		inline std::vector<RigidbodyContact>::iterator end()  { return contacts.end();}

		void reserve(size_t count);
		size_t size() const;
		bool empty() const;
		void clear();


		std::vector<RigidbodyContact> contacts;
		//int contactsCount;
	};
} // namespace Voxymore::Core

