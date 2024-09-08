//
// Created by ianpo on 31/07/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Physics/ObjectLayerPairFilter.hpp"
#include "Voxymore/Scene/Scene.forward.hpp"
#include "Voxymore/Scene/Entity.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace Voxymore::Core
{
	enum class RigidbodyType {
		/// (not moving or simulating)
		Static,
		/// (moved by forces) or
		Dynamic,
		/// (moved by velocities only).
		Kinematic,
	};

	static const std::string& RigidbodyTypeToString(RigidbodyType rt);
	static RigidbodyType RigidbodyTypeFromString(const std::string& rt);

	class RigidbodyComponent : public SelfAwareComponent<RigidbodyComponent>
	{
	public:
		inline static std::string GetName() { return "Rigidbody";}
		inline bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix) {return false;};
	public:
		void DeserializeComponent(YAML::Node& node, Entity e);
		void SerializeComponent(YAML::Emitter& out, Entity e);
		bool OnImGuiRender(Entity e);
	public:
		inline RigidbodyComponent() = default;
		inline ~RigidbodyComponent() = default;
	public:
		[[nodiscard]] JPH::BodyCreationSettings GetCreationSettings(const JPH::ShapeSettings* inShape, Vec3 pos, Quat rot) const;
		[[nodiscard]] JPH::BodyCreationSettings GetCreationSettings(const JPH::Shape* inShape, Vec3 pos, Quat rot) const;
		[[nodiscard]] JPH::BodyCreationSettings GetCreationSettings(const JPH::ShapeSettings* inShape, Entity e) const;
		[[nodiscard]] JPH::BodyCreationSettings GetCreationSettings(const JPH::Shape* inShape, Entity e) const;
	public:
		[[nodiscard]] JPH::ObjectLayer GetLayer() const;
		[[nodiscard]] JPH::EMotionType GetMotionType() const;
	public:
		// Getters
		[[nodiscard]] Vec3 GetLinearVelocity() const {return m_LinearVelocity;}
		[[nodiscard]] Vec3 GetRadAngularVelocity() const {return m_AngularVelocity * Math::DegToRad;}
		[[nodiscard]] Vec3 GetDegAngularVelocity() const {return m_AngularVelocity;}
		[[nodiscard]] Real GetFriction() const {return m_Friction;}
		[[nodiscard]] Real GetGravityFactor() const {return m_GravityFactor;}
		[[nodiscard]] bool GetIsAwake() const {return m_IsAwake;}
		[[nodiscard]] JPH::EActivation GetActivation() const {return m_IsAwake ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;}

		// Setters
		void SetLinearVelocity(Vec3 linearVelocity) {m_LinearVelocity = linearVelocity;}
		void SetAngularVelocity(Vec3 angularVelocity) {m_AngularVelocity = angularVelocity;}

		void SetFriction(Real friction) {m_Friction = friction;}
		void SetClampFriction(Real friction) {SetFriction(Math::Clamp01(friction));}
		void SetGravityFactor(Real gravityFactor) {m_GravityFactor = gravityFactor;}
		void DisableGravity() {m_GravityFactor = 0.0_R;}
		void SetIsAwake(bool isAwake) {m_IsAwake = isAwake;}
	public:
		/// Add force (unit: N) at center of mass for the next time step, will be reset after the next call to PhysicsSystem::Update
		void AddForce(Vec3 force);
		/// Add force (unit: N) to point in world space for the next time step, will be reset after the next call to PhysicsSystem::Update
		void AddForce(Vec3 force, Vec3 worldPosition);
//		/// Add force (unit: N) to point in local space for the next time step, will be reset after the next call to PhysicsSystem::Update
//		void AddForceAtLocalPosition(Vec3 force, Vec3 localPosition);
		/// Add torque (unit: N m) for the next time step, will be reset after the next call to PhysicsSystem::Update
		void AddTorque(Vec3 torque);
		/// Add impulse to center of mass (unit: kg m/s)
		void AddImpulse(Vec3 impulse);
		/// Add impulse to point in world space (unit: kg m/s)
		void AddImpulse(Vec3 impulse, Vec3 worldPosition);
		/// Add angular impulse in world space (unit: N m s)
		void AddAngularImpulse(Vec3 angularImpulse);
		/// Set velocity of body such that it will be positioned at targetPosition/Rotation in duration seconds.
		void MoveKinematic(Vec3 targetPosition, Quat targetRotation, Real duration);
		/// Add a movement during the integration (unit: m)
		void Move(Vec3 movement);
		/// Add a rotation during the integration (unit: rad)
		void Rotate(Vec3 angularRotation);
	private:
		RigidbodyType m_Type{RigidbodyType::Dynamic};
		Vec3 m_LinearVelocity{0_R};
		Vec3 m_AngularVelocity{0_R};
		Real m_Friction{0.5_R};
		Real m_GravityFactor{1_R};
		bool m_IsAwake{true};
	private:
		JPH::Body* m_Body{nullptr};
		JPH::BodyID m_BodyID{};
	private:
		friend class Scene;
	};

	struct RigidbodyInitialized
	{
	};

	struct RigidbodyDirty
	{
	};



} // namespace Voxymore::Core

