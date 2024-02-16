//
// Created by ianpo on 11/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/TimeStep.hpp"

namespace Voxymore::Core
{
	/**
	 * @class Rigidbody
	 *
	 * @brief Represents a rigid body in a physics simulation.
	 *
	 * The Rigidbody class is used to represent a physical object in a physics simulation.
	 * It stores various properties of the object such as its mass, position, orientation,
	 * velocity, and acceleration. It also provides methods for applying forces, calculating
	 * the transformation matrix, and manipulating the body's inertia tensor.
	 */
	class Rigidbody
	{
	public:
		inline Rigidbody() = default;
		inline ~Rigidbody() = default;
		Rigidbody(Real inverseMass, Real linearDamping, Vec3 position, Quat orientation, Mat3 inverseInertiaTensor);
	public:
		Mat4 CalculateTransformMatrix() const;
		Mat3 CalculateWorldInverseInertiaTensor() const;

		void SetInertiaTensor(const Mat3& inertiaTensor);
		void SetInverseInertiaTensor(const Mat3& inverseInertiaTensor);

		void AddForce(const Vec3& force);
		void AddAcceleration(const Vec3& acceleration);
		void ClearAccumulator();

		void AddForceAtPoint(const Vec3& force,const Vec3& point);
		void AddForceAtBodyPoint(const Vec3& force,const Vec3& bodyPoint);

		void AddAccelerationAtPoint(const Vec3& accel, const Vec3& point);
		void AddAccelerationAtBodyPoint(const Vec3& accel, const Vec3& bodyPoint);

		[[nodiscard]] Real GetInverseMass() const;
		Real& GetInverseMass();
		void SetInverseMass(Real inverseMass);

		[[nodiscard]] Real GetMass() const;
		void SetMass(Real mass);

		[[nodiscard]] bool HasFiniteMass() const;

		[[nodiscard]] Real GetLinearDamping() const;
		Real& GetLinearDamping();
		void SetLinearDamping(Real linearDamping);

		[[nodiscard]] const Vec3& GetPosition() const;
		Vec3& GetPosition();
		void SetPosition(const Vec3& position);

		[[nodiscard]] const Quat& GetOrientation() const;
		Quat& GetOrientation();
		void SetOrientation(const Quat& orientation);

		[[nodiscard]] const Vec3& GetLinearVelocity() const;
		Vec3& GetLinearVelocity();
		void SetLinearVelocity(const Vec3& linearVelocity);

		[[nodiscard]] const Vec3& GetAngularVelocity() const;
		Vec3& GetAngularVelocity();
		void SetAngularVelocity(const Vec3& angularVelocity);

	protected:
		/**
		 * Hold the inverse of the mass of the rigidbody. It is
		 * more useful to hold the inverse mass because integration is
		 * simpler, and because in real time simulation it is more useful
		 * to have bodies with infinite mass (immovable) than zero mass
		 * (unstable in numerical simulation)
		 */
		Real m_InverseMass;

		/**
		 * Holds the amount of damping applied to linear
		 * motion. Damping is required to remove energy added
		 * through numerical instability in the integrator.
		 */
		Real m_LinearDamping;

		/**
		 * Linear position in world space.
		 */
		Vec3 m_Position;

		/**
		 * Angular orientation in world space.
		 */
		Quat m_Orientation;

		/**
		 * Linear velocity in world space.
		 */
		Vec3 m_LinearVelocity;

		/***
		 * Angular velocity (or rotation) in world space.
		 */
		Vec3 m_AngularVelocity;

		/**
		 * Holds the inverse of the body's inertia tensor. The
		 * inertia tensor provided must not be degenerate
		 * (that would mean the body had zero inertia for
		 * spinning along one axis). As long as the tensor
		 * is finite, it will be invertible. The inverse tensor
		 * is used for similar reasons to the use of inverse
		 * mass.
		 *
		 * The inertia tensor, unlike the other variable that
		 * define the rigidbody, is given in body space (local space).
		 */
		Mat3 m_InverseInertiaTensor;

		/**
		 *   @brief Used to accumulate forces applied on the Rigidbody during a physics simulation step.
		 *
		 *   The 3-dimensional vector m_ForceAccumulate represents the total force currently applied to the rigid body.
		 *   This force is an accumulation of all the individual forces applied to the body.
		 *
		 *   During a simulation step, external entities or the physics engine apply forces to the body.
		 *   These forces are added (accumulated) to m_ForceAccumulate. This is based on the principle
		 *   that forces are additive - the net effect of multiple forces acting on a body
		 *   is the sum of their individual effects.
		 *
		 *   This force accumulation process allows for the resultant force to be calculated in one go before
		 *   the physics simulation uses it to update the rigid body's state (position, velocity).
		 *
		 *   Typically, after each simulation step, m_ForceAccumulate is cleared to accept a new set of forces
		 *   for the next simulation step.
		 */
		Vec3 m_ForceAccumulate;

		/**
		 * @brief Accumulation of torques applied to the Rigidbody during a physics simulation step.
		 *
		 * This Vec3 data member represents the total torque applied to the Rigidbody during the simulation step.
		 * Torque, in physics, is a measure of the force that can cause an object to rotate about an axis.
		 *
		 * The torques resulting from forces applied at specific points on the body during the physics simulation are accumulated into m_TorqueAccumulate.
		 * Once the simulation step is done, the accumulated torque is used to update the state of the Rigidbody, specifically its angular velocity.
		 *
		 * Typically, m_TorqueAccumulate is cleared after each simulation step so it can accept and accumulate new torques in the next step.
		 * The clearing is done by calling the 'ClearAccumulator()' method in the physics engine updates.
		 */
		Vec3 m_TorqueAccumulate;

		/**
		 * @brief Flag to check if the Rigidbody is awake in the simulation.
		 *
		 * This Boolean data member, m_IsAwake, represents the awake state of the Rigidbody during the physics simulation step.
		 * When a Rigidbody is awake, it is subject to the physics simulation, which includes handling forces, collisions, etc.
		 *
		 * If m_IsAwake is set to true, this indicates that the Rigidbody is active in the physics simulation and needs to be updated.
		 * This might be due to a force or torque being applied, or the Rigidbody might be in motion due to previous simulation steps.
		 *
		 * If m_IsAwake is set to false, the physics engine may choose to omit this Rigidbody from certain computations, improving performance.
		 * Note: The specific behavior may vary depending on the physics engine's design and implementation.
		 */
		bool m_IsAwake;
	};

} // namespace Voxymore::Core

