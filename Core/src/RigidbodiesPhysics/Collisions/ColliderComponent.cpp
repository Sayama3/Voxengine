//
// Created by ianpo on 15/03/2024.
//

#include "Voxymore/RigidbodiesPhysics/Components/ColliderComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/TypeHelpers.hpp"

#define DeserializeField(node, fieldVariable, fieldName, type, defaultValue) fieldVariable = node[fieldName].as<type>(defaultValue);

namespace Voxymore::Core
{
	void ColliderComponent::DeserializeComponent(YAML::Node& node)
	{
		PrimitiveCollider::Type t = (PrimitiveCollider::Type) node["Type"].as<int>(PrimitiveCollider::Type::Box);
		switch (t) {

			case PrimitiveCollider::Sphere: {
				m_Collider = Sphere();
				Sphere& sphere = std::get<Sphere>(m_Collider);
				DeserializeField(node, sphere.m_Radius, "Radius", Real, .5);
				break;
			}
			case PrimitiveCollider::Plane: {
				m_Collider = Plane();
				Plane& plane = std::get<Plane>(m_Collider);
				DeserializeField(node, plane.m_Normal, "Normal", Vec3, Vec3(0,1,0));
				DeserializeField(node, plane.m_Offset, "Offset", Real, 0);
				break;
			}
			case PrimitiveCollider::Box: {
				m_Collider = Box();
				Box& box = std::get<Box>(m_Collider);
				DeserializeField(node, box.m_HalfSize, "HalfSize", Vec3, Vec3(.5,.5,.5));
				break;
			}
		}
	}

	void ColliderComponent::SerializeComponent(YAML::Emitter& out)
	{
		PrimitiveCollider::Type t = std::visit(overloads{[](const Box& box){return PrimitiveCollider::Type::Box;}, [](const Sphere& sphere){return PrimitiveCollider::Type::Sphere;}, [](const Plane& plane){return PrimitiveCollider::Type::Plane;}}, m_Collider);
		out << KEYVAL("Type", (int)t);
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
		PrimitiveCollider::Type t = std::visit(overloads{[](const Box& box){return PrimitiveCollider::Type::Box;}, [](const Sphere& sphere){return PrimitiveCollider::Type::Sphere;}, [](const Plane& plane){return PrimitiveCollider::Type::Plane;}}, m_Collider);

		const char* elems[3] = {"Sphere","Plane","Box"};
		if(ImGui::Combo("Type", (int*)&t, elems, 3))
		{
			switch (t) {
				case PrimitiveCollider::Sphere: m_Collider = Sphere(); break;
				case PrimitiveCollider::Plane: m_Collider = Plane(); break;
				case PrimitiveCollider::Box: m_Collider = Box(); break;
			}
		}

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
