//
// Created by ianpo on 31/07/2024.
//

#include "Voxymore/Physics/ColliderComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore::Core::ColliderShapes
{
	const JPH::Shape* CreateShape(Box box)
	{
		return new JPH::BoxShape(JPH::Vec3Arg{box.Size.x * Real(0.5),box.Size.y * Real(0.5),box.Size.z * Real(0.5)});
	}
	const JPH::Shape* CreateShape(Sphere sphere)
	{
		return new JPH::SphereShape(sphere.Radius);
	}
	const JPH::Shape* CreateShape(Capsule capsule)
	{
		auto cylinderHeight = Math::Max(capsule.Height - (2*capsule.Radius), Real(0));
		if(cylinderHeight >= REAL_EPSILON)
			return new JPH::CapsuleShape(cylinderHeight * Real(0.5), capsule.Radius);
		else
			return new JPH::SphereShape(capsule.Radius);
	}
	const JPH::Shape* CreateShape(Cylinder cylinder)
	{
		return new JPH::CylinderShape(cylinder.Height * 0.5, cylinder.Radius);
	}
}

namespace Voxymore::Core
{
	void ColliderComponent::DeserializeComponent(YAML::Node& node, Entity e)
	{
		VXM_PROFILE_FUNCTION();
		std::string typeStr = node["Type"].as<std::string>(std::string{});
		auto it = std::find(TypeNames.begin(), TypeNames.end(), typeStr);
		Type shape = it != TypeNames.end() ? (Type)std::distance(TypeNames.begin(), it) : Type::Box;
		switch (shape) {
			case Box:
			{
				m_Shape = ColliderShapes::Box{};
				ColliderShapes::Box& shp = std::get<ColliderShapes::Box>(m_Shape);
				shp.Size = node["Size"].as<Vec3>(shp.Size);
			}
				break;
			case Sphere:
			{
				m_Shape = ColliderShapes::Sphere{};
				ColliderShapes::Sphere& shp = std::get<ColliderShapes::Sphere>(m_Shape);
				shp.Radius = node["Radius"].as<Real>(shp.Radius);
			}
				break;
			case Capsule:
			{
				m_Shape = ColliderShapes::Capsule{};
				ColliderShapes::Capsule& shp = std::get<ColliderShapes::Capsule>(m_Shape);
				shp.Radius = node["Radius"].as<Real>(shp.Radius);
				shp.Height = node["Height"].as<Real>(shp.Height);
			}
				break;
			case Cylinder:
			{
				m_Shape = ColliderShapes::Cylinder{};
				ColliderShapes::Cylinder& shp = std::get<ColliderShapes::Cylinder>(m_Shape);
				shp.Radius = node["Radius"].as<Real>(shp.Radius);
				shp.Height = node["Height"].as<Real>(shp.Height);
			}
				break;
		}
	}

	void ColliderComponent::SerializeComponent(YAML::Emitter& out, Entity e)
	{
		out << KEYVAL("Type", TypeNames.at(m_Shape.index()));
		Type shape = static_cast<Type>(m_Shape.index());
		switch (shape) {
			case Box:
			{
				ColliderShapes::Box& shp = std::get<ColliderShapes::Box>(m_Shape);
				out << KEYVAL("Size",shp.Size);
			}
				break;
			case Sphere:
			{
				ColliderShapes::Sphere& shp = std::get<ColliderShapes::Sphere>(m_Shape);
				out << KEYVAL("Radius",shp.Radius);
			}
				break;
			case Capsule:
			{
				ColliderShapes::Capsule& shp = std::get<ColliderShapes::Capsule>(m_Shape);
				out << KEYVAL("Radius",shp.Radius);
				out << KEYVAL("Height",shp.Height);
			}
				break;
			case Cylinder:
			{
				ColliderShapes::Cylinder& shp = std::get<ColliderShapes::Cylinder>(m_Shape);
				out << KEYVAL("Radius",shp.Radius);
				out << KEYVAL("Height",shp.Height);
			}
				break;
		}
	}
	bool ColliderComponent::OnImGuiRender(Entity e)
	{
		bool changed = false;
		bool sceneStarted = e.GetScene()?e.GetScene()->IsStarted() : false;
		Type shape = static_cast<Type>(m_Shape.index());
		if(ImGui::Combo("Collider Type", (int*)&shape, TypeNamesCombo) && shape != m_Shape.index())
		{
			changed = true;
			switch (shape) {
				case Box:
					m_Shape.emplace<ColliderShapes::Box>();
					break;
				case Sphere:
					m_Shape.emplace<ColliderShapes::Sphere>();
					break;
				case Capsule:
					m_Shape.emplace<ColliderShapes::Capsule>();
					break;
				case Cylinder:
					m_Shape.emplace<ColliderShapes::Cylinder>();
					break;
			}
		}

		switch (shape) {
			case Box:
			{
				ColliderShapes::Box& shp = std::get<ColliderShapes::Box>(m_Shape);
				changed |= ImGuiLib::DragReal3("Size##Box", Math::ValuePtr(shp.Size), 0.1, 0.1, REAL_MAX, "%.1f");
			}
				break;
			case Sphere:
			{
				ColliderShapes::Sphere& shp = std::get<ColliderShapes::Sphere>(m_Shape);
				changed |= ImGuiLib::DragReal("Radius##Sphere", &shp.Radius, 0.1, 0.1, REAL_MAX, "%.1f");
			}
				break;
			case Capsule:
			{
				ColliderShapes::Capsule& shp = std::get<ColliderShapes::Capsule>(m_Shape);
				changed |= ImGuiLib::DragReal("Radius##Capsule", &shp.Radius, 0.1, 0.1, REAL_MAX, "%.1f");
				changed |= ImGuiLib::DragReal("Height##Capsule", &shp.Height, 0.1, 0, REAL_MAX, "%.1f");
			}
				break;
			case Cylinder:
			{
				ColliderShapes::Cylinder& shp = std::get<ColliderShapes::Cylinder>(m_Shape);
				changed |= ImGuiLib::DragReal("Radius##Cylinder", &shp.Radius, 0.1, 0.1, REAL_MAX, "%.1f");
				changed |= ImGuiLib::DragReal("Height##Cylinder", &shp.Height, 0.1, 0.1, REAL_MAX, "%.1f");
			}
				break;
		}

		if(changed) {
			e.EnsureHasEmptyComponent<ColliderDirty>();
		}

		return changed;
	}

	const JPH::Shape *ColliderComponent::GetShape()
	{
		return std::visit([](auto&& args){return ColliderShapes::CreateShape(args);}, m_Shape);
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