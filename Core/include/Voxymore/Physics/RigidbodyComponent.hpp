//
// Created by ianpo on 31/07/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Physics/ObjectLayerPairFilter.hpp"
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
		Vec3 GetLinearVelocity() const {return m_LinearVelocity;}
		Vec3 GetAngularVelocity() const {return m_AngularVelocity;}
		Real GetFriction() const {return m_Friction;}
		Real GetGravityFactor() const {return m_GravityFactor;}
		bool GetIsAwake() const {return m_IsAwake;}
		JPH::EActivation GetActivation() {return m_IsAwake ? JPH::EActivation::Activate : JPH::EActivation::DontActivate;}

		// Setters
		void SetLinearVelocity(Vec3 linearVelocity) {m_LinearVelocity = linearVelocity;}
		void SetAngularVelocity(Vec3 angularVelocity) {m_AngularVelocity = angularVelocity;}
		void SetFriction(Real friction) {m_Friction = friction;}
		void SetGravityFactor(Real gravityFactor) {m_GravityFactor = gravityFactor;}
		void SetIsAwake(bool isAwake) {m_IsAwake = isAwake;}
	private:
		RigidbodyType m_Type{RigidbodyType::Dynamic};
		Vec3 m_LinearVelocity{0};
		Vec3 m_AngularVelocity{0};
		Real m_Friction{0.5};
		Real m_GravityFactor{1.0};
		bool m_IsAwake{true};
	};

	struct RigibodyIDComponent
	{
		JPH::BodyID BodyID;
	};

	struct RigidbodyDirty
	{
	};



} // namespace Voxymore::Core

