//
// Created by ianpo on 11/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	namespace Helper
	{

		/**
		 * Internal function to do an intertia tensor transform by a quaternion.
		 * Note that the implementation of this function was created by an
		 * automated code-generator and optimizer.
		 */
		static inline void _transformInertiaTensor(Mat3 &iitWorld,
												   const Quat &q,
												   const Mat3 &iitBody,
												   const Mat4 &rotmat)
		{
			VXM_PROFILE_FUNCTION();
			Real t4 = rotmat[0][0]*iitBody[0][0]+
					  rotmat[1][0]*iitBody[0][1]+
					  rotmat[2][0]*iitBody[0][2];
			Real t9 = rotmat[0][0]*iitBody[1][0]+
					  rotmat[1][0]*iitBody[1][1]+
					  rotmat[2][0]*iitBody[1][2];
			Real t14 = rotmat[0][0]*iitBody[2][0]+
					   rotmat[1][0]*iitBody[2][1]+
					   rotmat[2][0]*iitBody[2][2];

			Real t28 = rotmat[0][1]*iitBody[0][0]+
					   rotmat[1][1]*iitBody[0][1]+
					   rotmat[2][1]*iitBody[0][2];
			Real t33 = rotmat[0][1]*iitBody[1][0]+
					   rotmat[1][1]*iitBody[1][1]+
					   rotmat[2][1]*iitBody[1][2];
			Real t38 = rotmat[0][1]*iitBody[2][0]+
					   rotmat[1][1]*iitBody[2][1]+
					   rotmat[2][1]*iitBody[2][2];

			Real t52 = rotmat[0][2]*iitBody[0][0]+
					   rotmat[1][2]*iitBody[0][1]+
					   rotmat[2][2]*iitBody[0][2];
			Real t57 = rotmat[0][2]*iitBody[1][0]+
					   rotmat[1][2]*iitBody[1][1]+
					   rotmat[2][2]*iitBody[1][2];
			Real t62 = rotmat[0][2]*iitBody[2][0]+
					   rotmat[1][2]*iitBody[2][1]+
					   rotmat[2][2]*iitBody[2][2];

			iitWorld[0][0] = t4*rotmat[0][0]+
							 t9*rotmat[1][0]+
							 t14*rotmat[2][0];
			iitWorld[1][0] = t4*rotmat[0][1]+
							 t9*rotmat[1][1]+
							 t14*rotmat[2][1];
			iitWorld[2][0] = t4*rotmat[0][2]+
							 t9*rotmat[1][2]+
							 t14*rotmat[2][2];
			iitWorld[0][1] = t28*rotmat[0][0]+
							 t33*rotmat[1][0]+
							 t38*rotmat[2][0];
			iitWorld[1][1] = t28*rotmat[0][1]+
							 t33*rotmat[1][1]+
							 t38*rotmat[2][1];
			iitWorld[2][1] = t28*rotmat[0][2]+
							 t33*rotmat[1][2]+
							 t38*rotmat[2][2];
			iitWorld[0][2] = t52*rotmat[0][0]+
							 t57*rotmat[1][0]+
							   t62*rotmat[2][0];
			iitWorld[1][2] = t52*rotmat[0][1]+
							 t57*rotmat[1][1]+
							 t62*rotmat[2][1];
			iitWorld[2][2] = t52*rotmat[0][2]+
							 t57*rotmat[1][2]+
							 t62*rotmat[2][2];
		}
	}

	Rigidbody::Rigidbody(Real inverseMass, Real linearDamping, TransformComponent* transform, Mat3 inverseInertiaTensor)
		: m_InverseMass(inverseMass), m_LinearDamping(linearDamping), m_Transform(transform), m_InverseInertiaTensor(inverseInertiaTensor)
	{
	}

	void Rigidbody::Integrate(Real ts)
	{
		VXM_PROFILE_FUNCTION();

		if(!m_Transform) return;

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

		m_Transform->AddMovement(m_LinearVelocity * ts);
		auto rad = glm::radians(m_AngularVelocity * ts);
		auto orientation = m_Transform->GetRotation();
		auto qua = Quat();
		qua.x = rad.x;
		qua.y = rad.y;
		qua.z = rad.z;
		qua.w = 0;
		qua *= orientation;
		orientation += qua * (Real)0.5;
		orientation = glm::normalize(orientation);
		m_Transform->SetRotation(orientation);
		ClearAccumulator();
	}

	Mat4 Rigidbody::CalculateTransformMatrix() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Transform->GetTransform();
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

	const Mat3& Rigidbody::GetInverseInertiaTensor() const
	{
		return m_InverseInertiaTensor;
	}

	Mat3 Rigidbody::GetInverseInertiaTensorWorld() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Transform, "The transform component is not set.");
		Mat3 iitWorld;
		Helper::_transformInertiaTensor(iitWorld, m_Transform->GetRotation(), m_InverseInertiaTensor, m_Transform->GetTransform());
		return iitWorld;
	}

	void Rigidbody::GetInverseInertiaTensorWorld(Mat3& iitWorld) const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Transform, "The transform component is not set.");
		Helper::_transformInertiaTensor(iitWorld, m_Transform->GetRotation(), m_InverseInertiaTensor, m_Transform->GetTransform());
	}

	Mat3 Rigidbody::CalculateWorldInverseInertiaTensor() const
	{
		VXM_PROFILE_FUNCTION();
		return Math::ToMat3(m_Transform->GetRotation()) * m_InverseInertiaTensor;
	}

	void Rigidbody::AddForce(const Vec3 &force)
	{
		VXM_PROFILE_FUNCTION();
		m_ForceAccumulate += force;
		// TODO: Add an "awake" mechanism without a bool m_Awake>
		// m_IsAwake = true;
	}

	void Rigidbody::AddAcceleration(const Vec3 & accel)
	{
		VXM_PROFILE_FUNCTION();
		m_ForceAccumulate += HasFiniteMass() ? accel * GetMass() : accel;
		// TODO: Add an "awake" mechanism without a bool m_Awake>
		// m_IsAwake = true;
	}


	void Rigidbody::AddForceAtPoint(const Vec3& force,const Vec3& point)
	{
		VXM_PROFILE_FUNCTION();
		auto r = point - m_Transform->GetPosition();

		m_ForceAccumulate += force;
		m_TorqueAccumulate += Math::Cross(r, force);

		// TODO: Add an "awake" mechanism without a bool m_Awake>
		// m_IsAwake = true;
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

	void Rigidbody::SetTransform(TransformComponent* transform)
	{
		m_Transform = transform;
	}

	const TransformComponent* Rigidbody::GetTransform() const
	{
		return m_Transform;
	}

	TransformComponent* Rigidbody::GetTransform()
	{
		return m_Transform;
	}

	Vec3 Rigidbody::GetPosition() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Transform->GetPosition();
	}

	void Rigidbody::SetPosition(const Vec3& position)
	{
		VXM_PROFILE_FUNCTION();
		m_Transform->SetPosition(position);
	}

	void Rigidbody::AddMovement(const Vec3& movement)
	{
		VXM_PROFILE_FUNCTION();
		m_Transform->AddMovement(movement);
	}

	Quat Rigidbody::GetOrientation() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Transform->GetRotation();
	}

	void Rigidbody::SetOrientation(const Quat& orientation)
	{
		VXM_PROFILE_FUNCTION();
		m_Transform->SetRotation(orientation);
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

	Vec3 Rigidbody::GetLastFrameAcceleration() const
	{
		//TODO: see if I shouldn't cache the force applied...
		return m_Acceleration;
	}

} // namespace Voxymore::Core