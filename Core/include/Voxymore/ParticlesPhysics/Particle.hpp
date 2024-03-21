//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Math/Math.hpp"

namespace Voxymore::Core
{
	class Particle
	{
	public:
		Particle(const Vec3& position, const Vec3& velocity = Vec3(0.0), const Vec3& acceleration = Vec3(0.0), Real damping = 0.999, Real inverseMass = 1.0);
		Particle(const Vec3& position, const Vec3& ForceAccumulate, const Vec3& velocity = Vec3(0.0), const Vec3& acceleration = Vec3(0.0), Real damping = 0.999, Real inverseMass = 1.0);
		inline Particle() = default;
		inline ~Particle() = default;

		[[nodiscard]] const Vec3& GetPosition() const;
		[[nodiscard]] const Vec3& GetVelocity() const;
		[[nodiscard]] const Vec3& GetAcceleration() const;
		[[nodiscard]] Real GetDamping() const;
		[[nodiscard]] Real GetMass() const;
		[[nodiscard]] Real GetInverseMass() const;

		/**
		 * @brief Set the position of the particle.
		 *
		 * This function sets the position of the particle to the specified position.
		 *
		 * @param position The new position of the particle.
		 */
		void SetPosition(const Vec3& position);

		/**
		 * @brief Adds a movement vector to the particle's position.
		 *
		 * This function adds the given movement vector to the particle's current position,
		 * effectively changing the position of the particle.
		 *
		 * @param movement The movement vector to add to the particle's position.
		 */
		void AddMovement(const Vec3& movement);

		/**
		 * @brief Set the velocity of the particle.
		 *
		 * This function sets the velocity of the particle to the specified velocity.
		 *
		 * @param velocity The new velocity of the particle.
		 */
		void SetVelocity(const Vec3& velocity);

		/**
		 * @brief Adds a velocity vector to the particle's current velocity.
		 *
		 * This function adds the given velocity vector to the particle's current velocity,
		 * effectively changing the velocity of the particle.
		 *
		 * @param velocity The velocity vector to add to the particle's velocity.
		 */
		void AddVelocity(const Vec3& velocity);

		/**
		 * @brief Set the acceleration of the particle.
		 *
		 * This function sets the acceleration of the particle to the specified acceleration vector.
		 *
		 * @param acceleration The new acceleration of the particle.
		 */
		void SetAcceleration(const Vec3& acceleration);

		/**
		 * @brief Set the damping value for the particle.
		 *
		 * This function sets the damping value for the particle. The damping value determines the rate at which the particle's velocity decreases over time.
		 *
		 * @param damping The new damping value for the particle.
		 */
		void SetDamping(const Real& damping);

		/**
		 * @brief Set the mass of the particle.
		 *
		 * This function sets the mass of the particle to the specified mass.
		 *
		 * @param mass The new mass of the particle.
		 */
		void SetMass(const Real& mass);


		/**
		 * @brief Set the inverse mass of the particle.
		 *
		 * This function sets the inverse mass of the particle to the specified inverse mass.
		 *
		 * @param inverseMass The new inverse mass of the particle.
		 */
		void SetInverseMass(const Real& inverseMass);

		/**
		 * @brief Integrate the particle's position and velocity over a specified timestep.
		 *
		 * This function updates the position and velocity of the particle by integrating the current acceleration, force accumulation, damping, and inverse mass over a specified timestep.
		 *
		 * @param ts The timestep to integrate over.
		 */
		void Integrate(Real ts);

		/**
		 * @brief Adds an acceleration vector to the particle's force accumulator.
		 *
		 * This function adds the given acceleration vector to the particle's force accumulator by multiplying it
		 * by the current mass of the particle, effectively accumulating the forces acting on the particle.
		 *
		 * @param acceleration The acceleration vector to add to the particle's force accumulator.
		 */
		void AccumulateAcceleration(Vec3 acceleration);
		/**
		 * @brief Adds a force vector to the particle's force accumulator.
		 *
		 * This function adds the given force vector to the particle's force accumulator,
		 * effectively accumulating the forces acting on the particle.
		 *
		 * @param force The force vector to add to the particle's force accumulator.
		 */
		void AccumulateForce(Vec3 force);

		/**
		 * @brief Clear the force accumulator of the particle.
		 *
		 * This function sets the force accumulator of the particle to zero.
		 * It clears all the accumulated forces acting on the particle.
		 */
		void ClearAccumulator();
	protected:
		Vec3 m_Position = Vec3(0.0);
		Vec3 m_Velocity = Vec3(0.0);
		Vec3 m_Acceleration = Math::Gravity;
		Real m_Damping = 0.9;
		Real m_InverseMass = 1.0;
		Vec3 m_ForceAccumulate = Vec3(0);
	};
}