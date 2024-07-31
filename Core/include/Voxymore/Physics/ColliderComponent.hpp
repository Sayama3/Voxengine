//
// Created by ianpo on 31/07/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/HeightFieldShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <variant>

namespace Voxymore::Core
{
	namespace ColliderShapes {
		struct Box {Vec3 Size;};
		struct Sphere {Real Radius;};
		struct Capsule {Real HeightCylinder; Real Radius;};
		struct Cylinder {Real Height; Real Radius; Real ConvexRadius = Real(0.05);};
	}

	class ColliderComponent : public SelfAwareComponent<ColliderComponent>
	{
	public:
		inline static std::string GetName() { return "Collider";}
		inline bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix) {return false;};
	public:
		void DeserializeComponent(YAML::Node& node, Entity e);
		void SerializeComponent(YAML::Emitter& out, Entity e);
		bool OnImGuiRender(Entity e);
	public:
		inline ColliderComponent() = default;
		inline ~ColliderComponent() = default;
	public:
		const JPH::Shape* GetShape();
	public:
		std::variant<ColliderShapes::Box,
				ColliderShapes::Sphere,
				ColliderShapes::Capsule,
				ColliderShapes::Cylinder> m_Shape;
	};

	class MeshColliderComponent : public SelfAwareComponent<MeshColliderComponent>
	{
	public:
		inline static std::string GetName() { return "Mesh Collider";}
		bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix) {return false;};
	public:
		void DeserializeComponent(YAML::Node& node, Entity e);
		void SerializeComponent(YAML::Emitter& out, Entity e);
		bool OnImGuiRender(Entity e);
	public:
		inline MeshColliderComponent() = default;
		inline ~MeshColliderComponent() = default;
	public:
		const JPH::Shape* GetShape();
	public:
		JPH::Array<JPH::Triangle> m_Triangles;
	};

	class HeightFieldColliderComponent : public SelfAwareComponent<MeshColliderComponent>
	{
	public:
		inline static std::string GetName() { return "Height Field Collider";}
		bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix);
	public:
		void DeserializeComponent(YAML::Node& node, Entity e);
		void SerializeComponent(YAML::Emitter& out, Entity e);
		bool OnImGuiRender(Entity e);
	public:
		inline HeightFieldColliderComponent() = default;
		inline ~HeightFieldColliderComponent() = default;
	public:
		const JPH::Shape* GetShape();
	public:
		/// The height field is a surface defined by: mOffset + mScale * (x, mHeightSamples[y * mSampleCount + x], y).
		/// where x and y are integers in the range x and y e [0, mSampleCount - 1].
		Vec3 m_Offset{0};
		Vec3 m_Scale{1};
		uint32_t m_SampleCount = 0;

		/// An array of mSampleCount^2 height samples. Samples are stored in row major order, so the sample at (x, y) is at index y * mSampleCount + x.
		std::vector<Real> m_HeightSamples;
	};

	class ColliderDirty : public EmptyComponent<ColliderDirty>
	{
		VXM_IMPLEMENT_EMPTYCOMPONENT(ColliderDirty);
	};

} // namespace Voxymore::Core

