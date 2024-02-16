//
// Created by ianpo on 11/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	Rigidbody::Rigidbody(Real inverseMass, Real linearDamping, Vec3 position, Quat orientation, Mat3 inverseInertiaTensor)
		: m_InverseMass(inverseMass), m_LinearDamping(linearDamping), m_Position(position), m_Orientation(orientation), m_InverseInertiaTensor(inverseInertiaTensor)
	{

	}
	void Rigidbody::Integrate(Real ts)
	{
		VXM_PROFILE_FUNCTION();

		if(m_InverseMass <= 0) {
			VXM_CORE_WARNING("The inverse mass of {0} is not valid, not integrating.", m_InverseMass);
			return;
		}

		VXM_CORE_ASSERT(ts != 0, "The timestep is supposed to be different from 0.");

		// Update position from velocity (and acceleration)
		m_Position += m_AngularVelocity * ts;
//		m_Position += m_Acceleration * ((ts * ts) * ((Real)0.5));

		// Update the acceleartion later on so pre-creating a variable.
		Vec3 acc = {0,0,0};
		acc += m_ForceAccumulate * m_InverseMass;

		// Update linear velocity from acceleration
		m_AngularVelocity += acc * ts;

		// Damping the velocity.
		VXM_CORE_CHECK(m_LinearDamping <= 1, "The Damping has the value {0}.", m_LinearDamping);
		m_AngularVelocity *= Math::Pow(m_LinearDamping, ts);

		// Clear the forces
		ClearAccumulator();
	}

	Mat4 Rigidbody::CalculateTransformMatrix() const
	{
		return Math::TRS(m_Position, m_Orientation);
	}

	void Rigidbody::SetInertiaTensor(const Mat3 &inertiaTensor)
	{
		m_InverseInertiaTensor = Math::Inverse(inertiaTensor);
	}

	void Rigidbody::SetInverseInertiaTensor(const Mat3& inverseInertiaTensor)
	{
		m_InverseInertiaTensor = inverseInertiaTensor;
	}

	Mat3 Rigidbody::CalculateWorldInverseInertiaTensor() const
	{
		return Math::ToMat3(m_Orientation) * m_InverseInertiaTensor;
	}

	void Rigidbody::AddForce(const Vec3 &force)
	{
		m_ForceAccumulate += force;
		m_IsAwake = true;
	}

	void Rigidbody::AddAcceleration(const Vec3 & accel)
	{
		m_ForceAccumulate += HasFiniteMass() ? accel * GetMass() : accel;
		m_IsAwake = true;
	}


	void Rigidbody::AddForceAtPoint(const Vec3& force,const Vec3& point)
	{
		Vec3 pt = point;
		pt -= m_Position;

		m_ForceAccumulate += force;
		m_TorqueAccumulate += Math::Cross(pt, force);

		m_IsAwake = true;
	}

	void Rigidbody::AddForceAtBodyPoint(const Vec3& force,const Vec3& bodyPoint)
	{
		auto pointv4 = Vec4(bodyPoint,1);
		auto transform = CalculateTransformMatrix();
		auto point = transform * pointv4;
		AddForceAtPoint(force, point);
	}

	void Rigidbody::AddAccelerationAtPoint(const Vec3& accel,const Vec3& point)
	{
		auto force = HasFiniteMass() ? accel * GetMass() : accel;
		AddForceAtPoint(force, point);
	}

	void Rigidbody::AddAccelerationAtBodyPoint(const Vec3& accel,const Vec3& bodyPoint)
	{
		auto force = HasFiniteMass() ? accel * GetMass() : accel;
		AddForceAtBodyPoint(force, bodyPoint);
	}

	void Rigidbody::ClearAccumulator()
	{
		m_ForceAccumulate = Vec3(0, 0, 0);
		m_TorqueAccumulate = Vec3(0, 0, 0);
	}

	Real Rigidbody::GetInverseMass() const
	{
		return m_InverseMass;
	}

	Real& Rigidbody::GetInverseMass()
	{
		return m_InverseMass;
	}

	void Rigidbody::SetInverseMass(Real inverseMass)
	{
		m_InverseMass = inverseMass;
	}
	Real Rigidbody::GetMass() const
	{
		return ((Real)1) / m_InverseMass;
	}

	void Rigidbody::SetMass(Real mass)
	{
		m_InverseMass = ((Real)1) / mass;
	}

	bool Rigidbody::HasFiniteMass() const
	{
		return m_InverseMass > 0;
	}

	Real Rigidbody::GetLinearDamping() const
	{
		return m_LinearDamping;
	}

	Real& Rigidbody::GetLinearDamping()
	{
		return m_LinearDamping;
	}

	void Rigidbody::SetLinearDamping(Real linearDamping)
	{
		m_LinearDamping = linearDamping;
	}

	const Vec3& Rigidbody::GetPosition() const
	{
		return m_Position;
	}

	Vec3& Rigidbody::GetPosition()
	{
		return m_Position;
	}

	void Rigidbody::SetPosition(const Vec3& position)
	{
		m_Position = position;
	}
	void Rigidbody::AddMovement(const Vec3& movement)
	{
		m_Position += movement;
	}

	const Quat& Rigidbody::GetOrientation() const
	{
		return m_Orientation;
	}

	Quat& Rigidbody::GetOrientation()
	{
		return m_Orientation;
	}

	void Rigidbody::SetOrientation(const Quat& orientation)
	{
		m_Orientation = orientation;
	}

	const Vec3& Rigidbody::GetLinearVelocity() const
	{
		return m_LinearVelocity;
	}

	Vec3& Rigidbody::GetLinearVelocity()
	{
		return m_LinearVelocity;
	}

	void Rigidbody::SetLinearVelocity(const Vec3& linearVelocity)
	{
		m_LinearVelocity = linearVelocity;
	}
	void Rigidbody::AddLinearVelocity(const Vec3& linearVelocity)
	{
		m_LinearVelocity += linearVelocity;
	}

	const Vec3& Rigidbody::GetAngularVelocity() const
	{
		return m_AngularVelocity;
	}

	Vec3& Rigidbody::GetAngularVelocity()
	{
		return m_AngularVelocity;
	}

	void Rigidbody::SetAngularVelocity(const Vec3& angularVelocity)
	{
		m_AngularVelocity = angularVelocity;
	}

	void Rigidbody::AddAngularVelocity(const Vec3& angularVelocity)
	{
		m_AngularVelocity += angularVelocity;
	}

} // namespace Voxymore::Core