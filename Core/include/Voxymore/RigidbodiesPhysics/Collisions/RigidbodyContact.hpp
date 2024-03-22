//
// Created by ianpo on 16/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	class RBContactResolver;

	class RigidbodyContact
	{
		friend class RBContactResolver;
	public:
		inline RigidbodyContact() = default;
		inline ~RigidbodyContact() = default;

		void SetBodyData(Rigidbody* one, Rigidbody* two, Real friction, Real restitution);
		operator bool() const;
	private:
		void ApplyPositionChange(std::array<Vec3, 2>& linearChange, std::array<Vec3, 2>& angularChange, Real penetration);
		void ApplyVelocityChange(std::array<Vec3, 2>&linearChange, std::array<Vec3, 2>&angularChange);
		void CalculateInternals(TimeStep ts);

		void SwapBodies();
		void CalculateContactBasis();
		Vec3 CalculateLocalVelocity(uint32_t bodyIndex, TimeStep ts);
		void CalculateDesiredDeltaVelocity(TimeStep ts);

		Vec3 CalculateFrictionlessImpulse(const std::array<Mat3, 2>& inverseInertiaTensorWorld);
		Vec3 CalculateFrictionImpulse(const std::array<Mat3, 2>& inverseInertiaTensorWorld);

		[[nodiscard]] const Mat3& GetContactToWorld() const;
		[[nodiscard]] Mat3 GetWorldToContact() const;
	public:
		std::array<Rigidbody*, 2> bodies;

		/**
		 * Holds the position of the contact in world coordinates.
		 */
		Vec3 contactPoint = {};

		/**
		 * The direction of the contact in world space.
		 */
		Vec3 contactNormal = {};

		/**
		 * The depth of penetration at the contact.
		 */
		Real penetration = 0;

		Real friction = 0;

		Real restitution = 0;
	private:
		Mat3 m_ContactToWorld = Math::Identity<Mat3>();
		Vec3 m_ContactVelocity;
		Real m_DesiredDeltaVelocity;
		std::array<Vec3, 2> m_RelativeContactPosition;
		constexpr static const Real c_VelocityLimit = Real(0.25);
	};

	struct CollisionData
	{
		CollisionData();
		~CollisionData();
		Real friction;
		Real restitution;

		void AddContact(int i = 1);
		[[deprecated("use CollisionData::AddContact")]]
		RigidbodyContact* GetContact();
		RigidbodyContact* GetContact(int i);
		void AddContact(const RigidbodyContact& contact);

		inline std::vector<RigidbodyContact>::iterator begin()  { return contacts.begin();}
		inline std::vector<RigidbodyContact>::iterator end()  { return contacts.end();}

		void reserve(size_t count);
		[[nodiscard]] size_t size() const;
		[[nodiscard]] bool empty() const;
		void clear();


		std::vector<RigidbodyContact> contacts;
		//int contactsCount;
	};
} // namespace Voxymore::Core

