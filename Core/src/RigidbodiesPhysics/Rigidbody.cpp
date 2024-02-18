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
			return;
		}

		Vec3 acceleration = m_Acceleration;
		acceleration += m_ForceAccumulate * m_InverseMass;
		Vec3 anguarAccel = m_InverseInertiaTensor * m_TorqueAccumulate;

		m_LinearVelocity += acceleration * ts;
		m_AngularVelocity += anguarAccel * ts;

		m_LinearVelocity *= Math::Pow(m_LinearDamping, static_cast<Real>(ts));
		m_AngularVelocity *= Math::Pow(m_AngularDamping, static_cast<Real>(ts));

		m_Position += m_LinearVelocity * ts;
		auto rad = glm::radians(m_AngularVelocity * ts);
		auto qua = Quat();
		qua.x = rad.x;
		qua.y = rad.y;
		qua.z = rad.z;
		qua.w = 0;
		qua *= m_Orientation;
		m_Orientation += qua * (Real)0.5;
		m_Orientation = glm::normalize(m_Orientation);
		ClearAccumulator();
	}

	Mat4 Rigidbody::CalculateTransformMatrix() const
	{
		VXM_PROFILE_FUNCTION();
		return Math::TRS(m_Position, m_Orientation);
	}

	void Rigidbody::SetInertiaTensor(const Mat3 &inertiaTensor)
	{
		VXM_PROFILE_FUNCTION();
		m_InverseInertiaTensor = Math::Inverse(inertiaTensor);
	}

	void Rigidbody::SetInverseInertiaTensor(const Mat3& inverseInertiaTensor)
	{
		VXM_PROFILE_FUNCTION();
		m_InverseInertiaTensor = inverseInertiaTensor;
	}

	Mat3 Rigidbody::CalculateWorldInverseInertiaTensor() const
	{
		VXM_PROFILE_FUNCTION();
		return Math::ToMat3(m_Orientation) * m_InverseInertiaTensor;
	}

	void Rigidbody::AddForce(const Vec3 &force)
	{
		VXM_PROFILE_FUNCTION();
		m_ForceAccumulate += force;
		m_IsAwake = true;
	}

	void Rigidbody::AddAcceleration(const Vec3 & accel)
	{
		VXM_PROFILE_FUNCTION();
		m_ForceAccumulate += HasFiniteMass() ? accel * GetMass() : accel;
		m_IsAwake = true;
	}


	void Rigidbody::AddForceAtPoint(const Vec3& force,const Vec3& point)
	{
		VXM_PROFILE_FUNCTION();
		auto r = point - m_Position;

		m_ForceAccumulate += force;
		m_TorqueAccumulate += Math::Cross(r, force);

		m_IsAwake = true;
	}

	void Rigidbody::AddForceAtBodyPoint(const Vec3& force,const Vec3& bodyPoint)
	{
		VXM_PROFILE_FUNCTION();
		auto pointv4 = Vec4(bodyPoint,1);
		auto transform = CalculateTransformMatrix();
		auto point = transform * pointv4;
		AddForceAtPoint(force, point);
	}

	void Rigidbody::AddAccelerationAtPoint(const Vec3& accel,const Vec3& point)
	{
		VXM_PROFILE_FUNCTION();
		auto force = HasFiniteMass() ? accel * GetMass() : accel;
		AddForceAtPoint(force, point);
	}

	void Rigidbody::AddAccelerationAtBodyPoint(const Vec3& accel,const Vec3& bodyPoint)
	{
		VXM_PROFILE_FUNCTION();
		auto force = HasFiniteMass() ? accel * GetMass() : accel;
		AddForceAtBodyPoint(force, bodyPoint);
	}

	void Rigidbody::ClearAccumulator()
	{
		VXM_PROFILE_FUNCTION();
		m_ForceAccumulate = Vec3(0, 0, 0);
		m_TorqueAccumulate = Vec3(0, 0, 0);
	}

	Real Rigidbody::GetInverseMass() const
	{
		VXM_PROFILE_FUNCTION();
		return m_InverseMass;
	}

	Real& Rigidbody::GetInverseMass()
	{
		VXM_PROFILE_FUNCTION();
		return m_InverseMass;
	}

	void Rigidbody::SetInverseMass(Real inverseMass)
	{
		VXM_PROFILE_FUNCTION();
		m_InverseMass = inverseMass;
	}
	Real Rigidbody::GetMass() const
	{
		VXM_PROFILE_FUNCTION();
		return ((Real)1.0) / m_InverseMass;
	}

	void Rigidbody::SetMass(Real mass)
	{
		VXM_PROFILE_FUNCTION();
		m_InverseMass = ((Real)1) / mass;
	}

	bool Rigidbody::HasFiniteMass() const
	{
		VXM_PROFILE_FUNCTION();
		return m_InverseMass > 0;
	}

	Real Rigidbody::GetLinearDamping() const
	{
		VXM_PROFILE_FUNCTION();
		return m_LinearDamping;
	}

	Real& Rigidbody::GetLinearDamping()
	{
		VXM_PROFILE_FUNCTION();
		return m_LinearDamping;
	}

	void Rigidbody::SetLinearDamping(Real linearDamping)
	{
		VXM_PROFILE_FUNCTION();
		m_LinearDamping = linearDamping;
	}

	const Vec3& Rigidbody::GetPosition() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Position;
	}

	Vec3& Rigidbody::GetPosition()
	{
		VXM_PROFILE_FUNCTION();
		return m_Position;
	}

	void Rigidbody::SetPosition(const Vec3& position)
	{
		VXM_PROFILE_FUNCTION();
		m_Position = position;
	}
	void Rigidbody::AddMovement(const Vec3& movement)
	{
		VXM_PROFILE_FUNCTION();
		m_Position += movement;
	}

	const Quat& Rigidbody::GetOrientation() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Orientation;
	}

	Quat& Rigidbody::GetOrientation()
	{
		VXM_PROFILE_FUNCTION();
		return m_Orientation;
	}

	void Rigidbody::SetOrientation(const Quat& orientation)
	{
		VXM_PROFILE_FUNCTION();
		m_Orientation = orientation;
	}

	const Vec3& Rigidbody::GetLinearVelocity() const
	{
		VXM_PROFILE_FUNCTION();
		return m_LinearVelocity;
	}

	Vec3& Rigidbody::GetLinearVelocity()
	{
		VXM_PROFILE_FUNCTION();
		return m_LinearVelocity;
	}

	void Rigidbody::SetLinearVelocity(const Vec3& linearVelocity)
	{
		VXM_PROFILE_FUNCTION();
		m_LinearVelocity = linearVelocity;
	}
	void Rigidbody::AddLinearVelocity(const Vec3& linearVelocity)
	{
		VXM_PROFILE_FUNCTION();
		m_LinearVelocity += linearVelocity;
	}

	const Vec3& Rigidbody::GetAngularVelocity() const
	{
		VXM_PROFILE_FUNCTION();
		return m_AngularVelocity;
	}

	Vec3& Rigidbody::GetAngularVelocity()
	{
		VXM_PROFILE_FUNCTION();
		return m_AngularVelocity;
	}

	void Rigidbody::SetAngularVelocity(const Vec3& angularVelocity)
	{
		VXM_PROFILE_FUNCTION();
		m_AngularVelocity = angularVelocity;
	}

	void Rigidbody::AddAngularVelocity(const Vec3& angularVelocity)
	{
		VXM_PROFILE_FUNCTION();
		m_AngularVelocity += angularVelocity;
	}

} // namespace Voxymore::Core