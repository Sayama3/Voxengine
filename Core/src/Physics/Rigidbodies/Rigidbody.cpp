//
// Created by ianpo on 11/02/2024.
//

#include "Voxymore/Physics/Rigidbodies/Rigidbody.hpp"

namespace Voxymore::Core
{
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

} // namespace Voxymore::Core