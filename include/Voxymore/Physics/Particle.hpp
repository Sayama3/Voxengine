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

		void SetPosition(const Vec3& position);
		void SetVelocity(const Vec3& velocity);
		void SetAcceleration(const Vec3& acceleration);
		void SetDamping(const Real& damping);
		void SetMass(const Real& mass);
		void SetInverseMass(const Real& inverseMass);

		void Integrate(Real ts);

		void AddAcceleration(Vec3 acceleration);
		void AddForce(Vec3 force);

		void ClearAccumulator();
	protected:
		Vec3 m_Position = Vec3(0.0);
		Vec3 m_Velocity = Vec3(0.0);
		Vec3 m_Acceleration = Vec3(0.0);
		Real m_Damping = 0.9;
		Real m_InverseMass = 1.0;
		Vec3 m_ForceAccumulate = Vec3(0);
	};
}