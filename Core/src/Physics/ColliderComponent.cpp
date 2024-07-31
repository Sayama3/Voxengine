//
// Created by ianpo on 31/07/2024.
//

#include "Voxymore/Physics/ColliderComponent.hpp"

namespace Voxymore::Core
{
	void ColliderComponent::DeserializeComponent(YAML::Node& node, Entity e)
	{

	}
	void ColliderComponent::SerializeComponent(YAML::Emitter& out, Entity e)
	{

	}
	bool ColliderComponent::OnImGuiRender(Entity e)
	{
		return false;
	}

	const JPH::Shape *ColliderComponent::GetShape()
	{
		return new JPH::BoxShape({0.5,0.5,0.5});
	}

	void MeshColliderComponent::DeserializeComponent(YAML::Node& node, Entity e)
	{

	}
	void MeshColliderComponent::SerializeComponent(YAML::Emitter& out, Entity e)
	{

	}
	bool MeshColliderComponent::OnImGuiRender(Entity e)
	{
		return false;
	}
	const JPH::Shape* MeshColliderComponent::GetShape()
	{
//		JPH::Shape::ShapeResult result;
//		auto settings = JPH::MeshShapeSettings(m_Triangles);
//		JPH::MeshShape* mesh = new JPH::MeshShape(settings, result);
//		return mesh;
		return new JPH::BoxShape({0.5,0.5,0.5});
	}

	void HeightFieldColliderComponent::DeserializeComponent(YAML::Node& node, Entity e)
	{

	}
	void HeightFieldColliderComponent::SerializeComponent(YAML::Emitter& out, Entity e)
	{

	}
	bool HeightFieldColliderComponent::OnImGuiRender(Entity e)
	{
		return false;
	}
	bool HeightFieldColliderComponent::OnImGuizmo(Entity e, const float *viewMatrix, const float *projectionMatrix) {
		return false;
	}
	const JPH::Shape *HeightFieldColliderComponent::GetShape()
	{
		return new JPH::BoxShape({0.5,0.5,0.5});
	}
}// namespace Voxymore::Core