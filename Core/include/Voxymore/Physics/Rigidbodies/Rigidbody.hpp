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
	};

} // namespace Voxymore::Core

