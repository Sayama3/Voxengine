//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/TimeStep.hpp"

namespace Voxymore::Core
{
	class Particle
	{
	public:
		Particle(const Vec3& position, const Vec3& velocity = Vec3(0.0), const Vec3& acceleration = Vec3(0.0), Real damping = 0.999, Real mass = 1.0);
		inline Particle() = default;
		inline ~Particle() = default;

		const Vec3& GetPosition() const;
		const Vec3& GetVelocity() const;
		const Vec3& GetAcceleration() const;
		Real GetDamping() const;
		Real GetMass() const;
		Real GetInverseMass() const;

		void SetPosition(const Vec3& position);
		void SetVelocity(const Vec3& velocity);
		void SetAcceleration(const Vec3& acceleration);
		void SetDamping(const Real& damping);
		void SetMass(const Real& mass);
		void SetInverseMass(const Real& inverseMass);

		void Integrate(Real ts);
	protected:
		Vec3 m_Position = Vec3(0.0);
		Vec3 m_Velocity = Vec3(0.0);
		Vec3 m_Acceleration = Vec3(0.0);
		Real m_Damping = 0.999;
		Real m_InverseMass = 1.0;
	};
}