//
// Created by ianpo on 11/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/TimeStep.hpp"

namespace Voxymore::Core
{
	class Rigidbody
	{
	public:
		Mat4 CalculateTransformMatrix() const;
		Mat3 CalculateWorldInverseInertiaTensor() const;

		void SetInertiaTensor(const Mat3& inertiaTensor);
		void SetInverseInertiaTensor(const Mat3& inverseInertiaTensor);

		void AddForce(const Vec3& force);
		void ClearAccumulator();

		void AddForceAtPoint(const Vec3& force,const Vec3& point);
		void AddForceAtBodyPoint(const Vec3& force,const Vec3& bodyPoint);
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

		Vec3 m_ForceAccumulate;

		Vec3 m_TorqueAccumulate;

		bool m_IsAwake;
	};

} // namespace Voxymore::Core

