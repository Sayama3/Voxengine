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
		AddForceAtPoint(force, CalculateTransformMatrix() * Vec4(bodyPoint,1));
	}

	void Rigidbody::ClearAccumulator()
	{
		m_ForceAccumulate = Vec3(0, 0, 0);
		m_TorqueAccumulate = Vec3(0, 0, 0);
	}
} // namespace Voxymore::Core