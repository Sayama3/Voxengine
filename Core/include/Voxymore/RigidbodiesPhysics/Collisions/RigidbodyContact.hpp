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

			RigidbodyContact(Rigidbody* p0, Vec3 contactNormal, Real restitution, Real penetration);
			RigidbodyContact(Rigidbody* p0, Rigidbody* p1, Vec3 contactNormal, Real restitution, Real penetration);
			RigidbodyContact(Rigidbody* p[2], Vec3 contactNormal, Real restitution, Real penetration);
			RigidbodyContact(const std::array<Rigidbody*, 2>& p, Vec3 contactNormal, Real restitution, Real penetration);

			/**
		 * Hold the rigidbodies that are involved in the contact.
		 * The second can be nullptr for contact with scenery.
		 */
			std::array<Rigidbody*, 2> rigidbodies;

			/**
		 * The direction of the contact in world space.
		 */
			Vec3 contactNormal;

			/**
		 * The normal restitution coefficient at contact.
		 */
			Real restitution;

			/**
		 * The depth of penetration at the contact.
		 */
			Real penetration;
		public:

			/**
		 * Resolve this contact for both velocity and interpenetration.
		 * @param duration duration of the frame of the contact.
		 */
			void Resolve(TimeStep duration);

			/**
		 * Calculates the separating velocity at this contact.
		 * @return
		 */
			Real CalculateSeparatingVelocity() const;

		private:

			/**
		 * Resolves the velocity of the rigidbodies involved in this contact.
		 *
		 * @param duration The duration of the frame of the contact.
		 *
		 * This function resolves the velocity of the rigidbodies involved in this contact.
		 * The ResolveVelocity function calculates the separating velocity at the contact using the CalculateSeparatingVelocity function.
		 * It then applies the appropriate impulse to each rigidbody to ensure their velocities are adjusted according to the restitution coefficient.
		 * This adjustment allows the rigidbodies to separate and prevent them from sticking together after a collision.
		 *
		 * @note This function should be called before the ResolveInterpenetration function to ensure that the velocity adjustment is applied before resolving interpenetration.
		 */
			void ResolveVelocity(TimeStep duration);


			/**
		 * Resolves the interpenetration between the rigidbodies involved in this contact.
		 *
		 * @param duration The duration of the frame of the contact.
		 *
		 * This function resolves the interpenetration between the rigidbodies involved in this contact.
		 * Interpenetration occurs when two rigidbodies overlap in space due to a collision or external force.
		 * The ResolveInterpenetration function calculates the depth of penetration using the separation distance
		 * between the rigidbodies along the contact normal. It then moves the rigidbodies apart along the contact normal
		 * by applying an impulse proportional to the penetration depth. This separation step allows the rigidbodies
		 * to resolve their interpenetration and prevent them from being overlapped, ensuring a physically accurate simulation.
		 *
		 * @note This function should be called after the ResolveVelocity function to ensure that the velocity of the
		 * rigidbodies has been properly updated before resolving the interpenetration.
		 */
			void ResolveInterpenetration(TimeStep duration);
		};

	} // namespace Voxymore::Core

