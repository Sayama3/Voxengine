//
// Created by ianpo on 31/07/2024.
//

#include "Voxymore/Physics/RigidbodyComponent.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore::Core
{
	static const std::string& RigidbodyTypeToString(RigidbodyType rt)
	{
		switch (rt) {
			case RigidbodyType::Static:{
				static const std::string StaticStr{"Static"};
				return StaticStr;
			}
			case RigidbodyType::Dynamic:{
				static const std::string DynamicStr{"Dynamic"};
				return DynamicStr;
			}
			case RigidbodyType::Kinematic:{
				static const std::string KinematicStr{"Kinematic"};
				return KinematicStr;
			}
		}
		VXM_CORE_ASSERT(false, "The rigidbody type {} is Unknown.", (int)rt);
		return "Unknown";
	}

	static RigidbodyType RigidbodyTypeFromString(const std::string& rt)
	{
		static const std::string StaticStr{"Static"};
		static const std::string DynamicStr{"Dynamic"};
		static const std::string KinematicStr{"Kinematic"};

		if (rt == StaticStr) return RigidbodyType::Static;
		if (rt == DynamicStr) return RigidbodyType::Dynamic;
		if (rt == KinematicStr) return RigidbodyType::Kinematic;


		VXM_CORE_ASSERT(false, "The rigidbody type '{}' is Unknown.", rt);
		return RigidbodyType::Static;
	}

	JPH::BodyCreationSettings RigidbodyComponent::GetCreationSettings(const JPH::ShapeSettings *inShape, Vec3 pos, Quat rot) const
	{
		return {inShape, JPH::RVec3Arg{pos.x, pos.y, pos.z}, JPH::QuatArg{rot.x, rot.y, rot.z, rot.w}, GetMotionType(), GetLayer()};
	}

	JPH::BodyCreationSettings RigidbodyComponent::GetCreationSettings(const JPH::Shape *inShape, Vec3 pos, Quat rot) const
	{
		return {inShape, JPH::RVec3Arg{pos.x, pos.y, pos.z}, JPH::QuatArg{rot.x, rot.y, rot.z, rot.w}, GetMotionType(), GetLayer()};
	}

	JPH::BodyCreationSettings RigidbodyComponent::GetCreationSettings(const JPH::ShapeSettings *inShape, Entity e) const
	{
		auto t = e.GetComponent<TransformComponent>();
		return GetCreationSettings(inShape, t.GetPosition(), t.GetRotation());
	}

	JPH::BodyCreationSettings RigidbodyComponent::GetCreationSettings(const JPH::Shape *inShape, Entity e) const
	{
		auto t = e.GetComponent<TransformComponent>();
		return GetCreationSettings(inShape, t.GetPosition(), t.GetRotation());
	}

	JPH::ObjectLayer RigidbodyComponent::GetLayer() const
	{
		switch (m_Type) {
			case RigidbodyType::Dynamic: return PhysicalLayers::MOVING;
			case RigidbodyType::Static: return PhysicalLayers::NON_MOVING;
			case RigidbodyType::Kinematic: return PhysicalLayers::MOVING;
		}
		VXM_CORE_ASSERT(false, "The Rigidbody type {} is unknown.", (int) m_Type);
		return PhysicalLayers::NON_MOVING;
	}
	JPH::EMotionType RigidbodyComponent::GetMotionType() const
	{
		switch (m_Type) {
			case RigidbodyType::Dynamic: return JPH::EMotionType::Dynamic;
			case RigidbodyType::Static: return JPH::EMotionType::Static;
			case RigidbodyType::Kinematic: return JPH::EMotionType::Kinematic;
		}
		VXM_CORE_ASSERT(false, "The Rigidbody type {} is unknown.", (int) m_Type);
		return JPH::EMotionType::Static;
	}
	void RigidbodyComponent::DeserializeComponent(YAML::Node &node, Entity e)
	{
		m_Type = node["RigidbodyType"] ? RigidbodyTypeFromString(node["RigidbodyType"].as<std::string>("NoValue")) : RigidbodyType::Dynamic;
		m_LinearVelocity = node["LinearVelocity"].as<Vec3>(Vec3{0});
		m_AngularVelocity = node["AngularVelocity"].as<Vec3>(Vec3{0});
		m_Friction = node["Friction"].as<Real>(Real{0.5});
		m_GravityFactor = node["GravityFactor"].as<Real>(Real{1});
		m_IsAwake = node["IsAwake"].as<bool>(true);
	}

	void RigidbodyComponent::SerializeComponent(YAML::Emitter &out, Entity e)
	{
		VXM_PROFILE_FUNCTION();
//		out << KEYVAL("InverseMass", m_InverseMass);
//		out << KEYVAL("LinearDamping", m_LinearDamping);
//		out << KEYVAL("AngularDamping", m_AngularDamping);
		out << KEYVAL("RigidbodyType", RigidbodyTypeToString(m_Type));
		out << KEYVAL("LinearVelocity", m_LinearVelocity);
		out << KEYVAL("AngularVelocity", m_AngularVelocity);
		out << KEYVAL("Friction", m_Friction);
		out << KEYVAL("GravityFactor", m_GravityFactor);
//		out << KEYVAL("Acceleration", m_Acceleration);
		out << KEYVAL("IsAwake", m_IsAwake);
	}

	bool RigidbodyComponent::OnImGuiRender(Entity e)
	{
		bool changed{false};
		changed |= ImGui::Combo("Rigidbody Type", (int*)&m_Type, "Static\0Dynamic\0Kinematic");
		changed |= ImGuiLib::DrawVec3Control("Linear Velocity", m_LinearVelocity);
		changed |= ImGuiLib::DrawVec3Control("Angular Velocity", m_AngularVelocity);
		changed |= ImGuiLib::DragReal("Friction", &m_Friction);
		changed |= ImGuiLib::DragReal("Gravity Factor", &m_GravityFactor);
		changed |= ImGui::Checkbox("IsAwake", &m_IsAwake);
		return changed;
	}
}// namespace Voxymore::Core