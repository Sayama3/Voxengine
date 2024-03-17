//
// Created by ianpo on 15/03/2024.
//

#include "Voxymore/RigidbodiesPhysics/Components/ColliderComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/TypeHelpers.hpp"


namespace Voxymore::Core
{
	void ColliderComponent::DeserializeComponent(YAML::Node& node)
	{
		auto useSphere = [&node](Sphere& sphere) -> void {
			auto rad = node["Radius"];
			if(rad.IsDefined())
			{
				sphere.m_Radius = rad.as<Real>();
			}
			else
			{
				sphere.m_Radius = 1;
			}
		};
		auto useBox = [&node](Box& box) -> void {
			auto half = node["HalfSize"];
			if(half.IsDefined())
			{
				box.m_HalfSize = half.as<Vec3>();
			}
			else
			{
				box.m_HalfSize = {0,0,0};
			}
		};
		auto usePlane = [&node](Plane& plane) -> void {
			auto norm = node["Normal"];
			if(norm.IsDefined())
			{
				plane.m_Normal = norm.as<Vec3>();
			}
			else
			{
				plane.m_Normal = {0,0,0};
			}

			auto offset = node["Offset"];
			if(offset.IsDefined())
			{
				plane.m_Offset = offset.as<Real>();
			}
			else
			{
				plane.m_Offset = 0;
			}
		};
		std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	void ColliderComponent::SerializeComponent(YAML::Emitter& out)
	{
		auto useSphere = [&out](const Sphere& sphere) -> void {
			out << KEYVAL("Radius", sphere.m_Radius);
		};
		auto useBox = [&out](const Box& box) -> void {
			out << KEYVAL("HalfSize", box.m_HalfSize);
		};
		auto usePlane = [&out](const Plane& plane) -> void {
			out << KEYVAL("Normal",plane.m_Normal);
			out << KEYVAL("Offset",plane.m_Offset);
		};
		std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	bool ColliderComponent::OnImGuiRender()
	{
		auto useSphere = [](Sphere& sphere) -> bool {
			return ImGuiLib::DragReal("Radius", &sphere.m_Radius);
		};
		auto useBox = [](Box& box) -> bool {
			return ImGuiLib::DragReal3("Half Size", glm::value_ptr(box.m_HalfSize));
		};
		auto usePlane = [](Plane& plane) -> bool {
			bool changed = false;
			changed |=  ImGuiLib::DragReal3("Normal", glm::value_ptr(plane.m_Normal));
			changed |=  ImGuiLib::DragReal("Offset", &plane.m_Offset);
			return changed;
		};
		return std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	BoundingSphere ColliderComponent::GetBoundingSphere() const
	{
		auto useSphere = [](const Sphere& sphere) -> BoundingSphere { return BoundingSphere(sphere.GetPosition(), sphere.m_Radius);};
		auto useBox = [](const Box& box) -> BoundingSphere { return BoundingSphere(box.GetVertices());};
		auto usePlane = [](const Plane& plane) -> BoundingSphere { return BoundingSphere(plane.GetPosition(), -1);};

		return std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	BoundingBox ColliderComponent::GetBoundingBox() const
	{
		auto useSphere = [](const Sphere& sphere) -> BoundingBox { return BoundingBox(sphere.GetPosition() - Vec3(sphere.m_Radius), sphere.GetPosition() + Vec3(sphere.m_Radius));};
		auto useBox = [](const Box& box) -> BoundingBox { return BoundingBox(box.GetVertices());};
		auto usePlane = [](const Plane& plane) -> BoundingBox { return BoundingBox();};

		return std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}
} // namespace Voxymore::Core
