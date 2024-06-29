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
	public:
		Vec3 GetContactVelocity() const {return m_ContactVelocity;}
		Real GetDesiredDeltaVelocity() const {return m_DesiredDeltaVelocity;}
		std::array<Vec3, 2> GetRelativeContactPosition() const {return m_RelativeContactPosition;}
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
		Vec3 contactPoint = glm::vec3{0,0,0};

		/**
		 * The direction of the contact in world space.
		 */
		Vec3 contactNormal = glm::vec3{0,0,0};

		/**
		 * The depth of penetration at the contact.
		 */
		Real penetration = 0;

		Real friction = 0;

		Real restitution = 0;
	private:
		Mat3 m_ContactToWorld = Math::Identity<Mat3>();
		Vec3 m_ContactVelocity = glm::vec3{0,0,0};
		Real m_DesiredDeltaVelocity = 0 ;
		std::array<Vec3, 2> m_RelativeContactPosition = {glm::vec3{0,0,0}, glm::vec3{0,0,0}};
		static constexpr Real c_VelocityLimit = Real(0.25);
	};

	//TODO: Create Default Collision Data and customizable one
	struct CollisionData
	{
	public:
		using iterator = std::vector<RigidbodyContact>::iterator;
		using const_iterator = std::vector<RigidbodyContact>::const_iterator;
	public:
		CollisionData();
		~CollisionData();
		Real friction = 0.6;
		Real restitution = .8;

		void AddContact(int i = 1);

		[[deprecated("use CollisionData::AddContact")]]
		RigidbodyContact* GetContact();

		RigidbodyContact* GetContact(int i);

		RigidbodyContact& operator[](uint64_t index);
		const RigidbodyContact& operator[](uint64_t index) const;

		void AddContact(const RigidbodyContact& contact);

		inline iterator begin()  { return contacts.begin();}
		inline iterator end()  { return contacts.end();}

		inline const_iterator begin() const  { return contacts.cbegin();}
		inline const_iterator end() const  { return contacts.cend();}

		inline const_iterator cbegin() const  { return contacts.cbegin();}
		inline const_iterator cend() const  { return contacts.cend();}

		void reserve(size_t count);
		[[nodiscard]] size_t size() const;
		[[nodiscard]] bool empty() const;
		void clear();

		std::vector<RigidbodyContact> contacts;
		//int contactsCount;
	};
} // namespace Voxymore::Core

