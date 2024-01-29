//
// Created by ianpo on 22/01/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Physics/Particle.hpp"

namespace Voxymore::Core
{
	/**
	 * Class representing the contact between a particle and a scenerie or another particle.
	 */
	class ParticleContact
	{
	public:
		inline ParticleContact() = default;
		inline ~ParticleContact() = default;

		ParticleContact(Particle* p0, Vec3 contactNormal, Real restitution, Real penetration);
		ParticleContact(Particle* p0, Particle* p1, Vec3 contactNormal, Real restitution, Real penetration);
		ParticleContact(Particle* p[2], Vec3 contactNormal, Real restitution, Real penetration);
		ParticleContact(const std::array<Particle*, 2>& p, Vec3 contactNormal, Real restitution, Real penetration);

		/**
		 * Hold the particles that are involved in the contact.
		 * The second can be nullptr for contact with scenery.
		 */
		std::array<Particle*, 2> particles;

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
		 * Resolves the velocity of the particles involved in this contact.
		 *
		 * @param duration The duration of the frame of the contact.
		 *
		 * This function resolves the velocity of the particles involved in this contact.
		 * The ResolveVelocity function calculates the separating velocity at the contact using the CalculateSeparatingVelocity function.
		 * It then applies the appropriate impulse to each particle to ensure their velocities are adjusted according to the restitution coefficient.
		 * This adjustment allows the particles to separate and prevent them from sticking together after a collision.
		 *
		 * @note This function should be called before the ResolveInterpenetration function to ensure that the velocity adjustment is applied before resolving interpenetration.
		 */
		void ResolveVelocity(TimeStep duration);


		/**
		 * Resolves the interpenetration between the particles involved in this contact.
		 *
		 * @param duration The duration of the frame of the contact.
		 *
		 * This function resolves the interpenetration between the particles involved in this contact.
		 * Interpenetration occurs when two particles overlap in space due to a collision or external force.
		 * The ResolveInterpenetration function calculates the depth of penetration using the separation distance
		 * between the particles along the contact normal. It then moves the particles apart along the contact normal
		 * by applying an impulse proportional to the penetration depth. This separation step allows the particles
		 * to resolve their interpenetration and prevent them from being overlapped, ensuring a physically accurate simulation.
		 *
		 * @note This function should be called after the ResolveVelocity function to ensure that the velocity of the
		 * particles has been properly updated before resolving the interpenetration.
		 */
		void ResolveInterpenetration(TimeStep duration);
	};
} // namespace Voxymore::Core

