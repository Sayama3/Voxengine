//
// Created by ianpo on 11/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Components/Components.hpp"

namespace Voxymore::Core
{
	/**
	 * @class Rigidbody
	 * @brief Represents a rigid body in a physics simulation.
	 *
	 * The Rigidbody class stores and updates the position, orientation, velocities, forces, and other properties
	 * of a rigid body. It also provides methods to perform various calculations and operations on the body.
	 */
	class Rigidbody
	{
	public:
		inline Rigidbody() = default;
		inline ~Rigidbody() = default;
		Rigidbody(Real inverseMass, Real linearDamping, TransformComponent* transform, Mat3 inverseInertiaTensor);
	public:
		/**
		 * @brief Integrate the rigid body over a specified time step.
		 *
		 * This method updates the position and orientation of the rigid body based on
		 * its current state and the specified time step.
		 *
		 * @param ts The time step for integration.
		 */
		void Integrate(Real ts);
	public:
		Mat4 CalculateTransformMatrix() const;
		Mat3 CalculateWorldInverseInertiaTensor() const;

		void SetInertiaTensor(const Mat3& inertiaTensor);
		void SetInverseInertiaTensor(const Mat3& inverseInertiaTensor);
		const Mat3& GetInverseInertiaTensor() const;
		Mat3 GetInverseInertiaTensorWorld() const;
		void GetInverseInertiaTensorWorld(Mat3& iitWorld) const;

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

		void SetTransform(TransformComponent* transform);
		const TransformComponent* GetTransform() const;
		TransformComponent* GetTransform();

		[[nodiscard]] Vec3 GetPosition() const;
		void SetPosition(const Vec3& position);
		void AddMovement(const Vec3& movement);

		[[nodiscard]] Quat GetOrientation() const;
		void SetOrientation(const Quat& orientation);

		[[nodiscard]] const Vec3& GetLinearVelocity() const;
		Vec3& GetLinearVelocity();
		void SetLinearVelocity(const Vec3& linearVelocity);
		void AddLinearVelocity(const Vec3& linearVelocity);

		[[nodiscard]] const Vec3& GetAngularVelocity() const;
		Vec3& GetAngularVelocity();
		void SetAngularVelocity(const Vec3& angularVelocity);
		void AddAngularVelocity(const Vec3& angularVelocity);

		Vec3 GetLastFrameAcceleration() const;

	protected:

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
		Mat3 m_InverseInertiaTensor = Math::Identity<Mat3>();

		/**
		* @brief Holds the acceleration vector applied to the rigid body.
		*
		* Acceleration is a vector quantity that represents the change in velocity of a body per unit of time.
		* This could result from any force or impulse applied to the body.
		* The more the value of acceleration, the faster the velocity of an object will change.
		* This is a vector value defined in the terms of distance travelled per unit of time squared.
		*/
		Vec3 m_Acceleration = Math::Gravity;

		/**
		 * Linear velocity in world space.
		 */
		Vec3 m_LinearVelocity = Vec3(0.0);

		/***
		 * Angular velocity (or rotation) in world space.
		 */
		Vec3 m_AngularVelocity = Vec3(0.0);

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
		Vec3 m_ForceAccumulate = Vec3(0.0);

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
		Vec3 m_TorqueAccumulate = Vec3(0.0);

		TransformComponent* m_Transform = nullptr;

		/**
		 * Hold the inverse of the mass of the rigidbody. It is
		 * more useful to hold the inverse mass because integration is
		 * simpler, and because in real time simulation it is more useful
		 * to have bodies with infinite mass (immovable) than zero mass
		 * (unstable in numerical simulation)
		 */
		Real m_InverseMass = 1.0;

		/**
		 * Holds the amount of damping applied to linear
		 * motion. Damping is required to remove energy added
		 * through numerical instability in the integrator.
		 */
		Real m_LinearDamping = 0.9;

		/**
		* @brief Holds the amount of angular damping applied to the rigid body.
		*
		* Angular damping is a force that reduces angular velocity over time, simulating the effects of resistance or friction.
		* This is necessary to prevent bodies from perpetually rotating due to applied forces and torques.
		* This damping helps remove energy that might be added through numerical instability in the simulations.
		* The more the value of angular damping, the faster an object will cease spinning after an applied force is removed.
		* This is a scalar value, with its unit being force time per radian.
		*/
		Real m_AngularDamping = 0.9;
	};

} // namespace Voxymore::Core

