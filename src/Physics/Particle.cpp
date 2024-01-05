//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/Particle.hpp"

namespace Voxymore::Core
{
	Particle::Particle(const Vec3& position, const Vec3& velocity, const Vec3& acceleration, Real damping, Real mass) : m_Position(position), m_Velocity(velocity), m_Acceleration(acceleration), m_Damping(damping), m_InverseMass(1.0)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(mass != 0, "The mass cannot be 0.");
		if(mass != 0)
		{
			m_InverseMass = 1.0 / mass;
		}
	}

	const Vec3& Particle::GetPosition() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Position;
	}

	void Particle::SetPosition(const Vec3& position)
	{
		VXM_PROFILE_FUNCTION();
		m_Position = position;
	}

	const Vec3& Particle::GetVelocity() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Velocity;
	}

	void Particle::SetVelocity(const Vec3& velocity)
	{
		VXM_PROFILE_FUNCTION();
		m_Velocity = velocity;
	}

	const Vec3& Particle::GetAcceleration() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Acceleration;
	}

	void Particle::SetAcceleration(const Vec3& acceleration)
	{
		VXM_PROFILE_FUNCTION();
		m_Acceleration = acceleration;
	}

	Real Particle::GetDamping() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Damping;
	}

	void Particle::SetDamping(const Real& damping)
	{
		VXM_PROFILE_FUNCTION();
		m_Damping = damping;
	}

	Real Particle::GetMass() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_InverseMass != 0, "The mass cannot be 0.");
		if (m_InverseMass != 0) {
			return ((Real)1.0) / m_InverseMass;
		}
		else {
			return 0.0;
		}
	}

	Real Particle::GetInverseMass() const
	{
		return m_InverseMass;
	}

	void Particle::SetMass(const Real& mass)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(mass != 0, "The mass cannot be 0.");
		if(mass == 0){
			m_InverseMass = 0;
		}
		else {
			m_InverseMass = 1.0 / mass;
		}
	}

	void Particle::SetInverseMass(const Real& inverseMass)
	{
		VXM_PROFILE_FUNCTION();
		m_InverseMass = inverseMass;
	}

	void Particle::Integrate(Real ts)
	{
		VXM_PROFILE_FUNCTION();

		if(m_InverseMass <= 0) {
			VXM_CORE_WARNING("The inverse mass of {0} is not valid, not integrating.", m_InverseMass);
			return;
		}

		VXM_CORE_ASSERT(ts != 0, "The timestep is supposed to be different from 0.");

		// Update position from velocity (and acceleration)
		m_Position += m_Velocity * ts;
		m_Position += m_Acceleration * ((ts * ts) * ((Real)0.5));

		// Update the acceleartion later on so pre-creating a variable.
		Vec3 acc = m_Acceleration;

		// Update linear velocity from acceleration
		m_Velocity += acc * ts;

		// Damping the velocity.
		VXM_CORE_CHECK(m_Damping <= 1, "The Damping has the value {0}.", m_Damping);
		m_Velocity *= Math::Pow(m_Damping, ts);

		// Clear the forces
		//TODO: clearAccumulator();
	}

}