//
// Created by ianpo on 15/03/2024.
//

#include "Voxymore/RigidbodiesPhysics/Components/ColliderComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/TypeHelpers.hpp"

#define DeserializeField(node, fieldVariable, fieldName, type, defaultValue) fieldVariable = node[fieldName].as<type>(defaultValue);

namespace Voxymore::Core
{
	void ColliderComponent::DeserializeComponent(YAML::Node& node, Entity e)
	{
		VXM_PROFILE_FUNCTION();
		PrimitiveCollider::Type t = (PrimitiveCollider::Type) node["Type"].as<int>(PrimitiveCollider::Type::Box);
		switch (t) {
			case PrimitiveCollider::Sphere: {
				m_Collider = Sphere();
				Sphere& sphere = std::get<Sphere>(m_Collider);
				DeserializeField(node, sphere.m_Radius, "Radius", Real, .5);
				sphere.m_Body = e.HasComponent<RigidbodyComponent>() ? reinterpret_cast<Rigidbody*>(&e.GetComponent<RigidbodyComponent>()) : nullptr;
				sphere.m_Transform = &e.GetComponent<TransformComponent>();
				break;
			}
			case PrimitiveCollider::Plane: {
				m_Collider = Plane();
				Plane& plane = std::get<Plane>(m_Collider);
				DeserializeField(node, plane.m_Normal, "Normal", Vec3, Vec3(0,1,0));
				DeserializeField(node, plane.m_Offset, "Offset", Real, 0);
				DeserializeField(node, plane.m_Size, "Size", Real, 1);
				plane.m_Body = e.HasComponent<RigidbodyComponent>() ? reinterpret_cast<Rigidbody*>(&e.GetComponent<RigidbodyComponent>()) : nullptr;
				plane.m_Transform = &e.GetComponent<TransformComponent>();
				break;
			}
			case PrimitiveCollider::Box: {
				m_Collider = Box();
				Box& box = std::get<Box>(m_Collider);
				DeserializeField(node, box.m_HalfSize, "HalfSize", Vec3, Vec3(.5,.5,.5));
				box.m_Body = e.HasComponent<RigidbodyComponent>() ? reinterpret_cast<Rigidbody*>(&e.GetComponent<RigidbodyComponent>()) : nullptr;
				box.m_Transform = &e.GetComponent<TransformComponent>();
				break;
			}
		}
	}

	void ColliderComponent::SerializeComponent(YAML::Emitter& out, Entity e)
	{
		VXM_PROFILE_FUNCTION();
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
			out << KEYVAL("Size",plane.m_Size);
		};
		std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	bool ColliderComponent::OnImGuiRender(Entity e)
	{
		VXM_PROFILE_FUNCTION();
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

		auto useSphere = [&e](Sphere& sphere) -> bool {
			if(!sphere.m_Body) sphere.m_Body = e.HasComponent<RigidbodyComponent>() ? reinterpret_cast<Rigidbody*>(&e.GetComponent<RigidbodyComponent>()) : nullptr;
			if(!sphere.m_Transform) sphere.m_Transform = &e.GetComponent<TransformComponent>();
			return ImGuiLib::DragReal("Radius", &sphere.m_Radius);
		};
		auto usePlane = [&e](Plane& plane) -> bool {
			if(!plane.m_Body) plane.m_Body = e.HasComponent<RigidbodyComponent>() ? reinterpret_cast<Rigidbody*>(&e.GetComponent<RigidbodyComponent>()) : nullptr;
			if(!plane.m_Transform) plane.m_Transform = &e.GetComponent<TransformComponent>();
			bool changed = false;
			changed |=  ImGuiLib::DragReal3("Normal", glm::value_ptr(plane.m_Normal));
			changed |=  ImGuiLib::DragReal("Offset", &plane.m_Offset);
			changed |=  ImGuiLib::DragReal("Size", &plane.m_Size);
			return changed;
		};
		auto useBox = [&e](Box& box) -> bool {
			if(!box.m_Body) box.m_Body = e.HasComponent<RigidbodyComponent>() ? reinterpret_cast<Rigidbody*>(&e.GetComponent<RigidbodyComponent>()) : nullptr;
			if(!box.m_Transform) box.m_Transform = &e.GetComponent<TransformComponent>();
			return ImGuiLib::DragReal3("Half Size", glm::value_ptr(box.m_HalfSize));
		};
		return std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	BoundingSphere ColliderComponent::GetBoundingSphere() const
	{
		VXM_PROFILE_FUNCTION();
		auto useSphere = [](const Sphere& sphere) -> BoundingSphere { return BoundingSphere(sphere.GetPosition(), sphere.m_Radius);};
		auto useBox = [](const Box& box) -> BoundingSphere { return BoundingSphere(box.GetVertices());};
		auto usePlane = [](const Plane& plane) -> BoundingSphere { return BoundingSphere(plane.GetPosition(), plane.m_Size);};

		return std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	BoundingBox ColliderComponent::GetBoundingBox() const
	{
		VXM_PROFILE_FUNCTION();
		auto useSphere = [](const Sphere& sphere) -> BoundingBox { return BoundingBox(sphere.GetPosition() - Vec3(sphere.m_Radius), sphere.GetPosition() + Vec3(sphere.m_Radius));};
		auto useBox = [](const Box& box) -> BoundingBox { return BoundingBox(box.GetVertices());};
		auto usePlane = [](const Plane& plane) -> BoundingBox {const auto& pos = plane.GetPosition(); auto hf = glm::vec3(plane.m_Size); return BoundingBox(pos - hf, pos + hf);};

		return std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	void ColliderComponent::SetTransform(TransformComponent* tc)
	{
		VXM_PROFILE_FUNCTION();
		auto useSphere = [tc](Sphere& sphere) -> void { sphere.m_Transform = tc; sphere.CacheMatrix(); };
		auto useBox = [tc](Box& box) -> void { box.m_Transform = tc; box.CacheMatrix(); };
		auto usePlane = [tc](Plane& plane) -> void { plane.m_Transform = tc; plane.CacheMatrix(); };

		std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}

	void ColliderComponent::SetRigidbody(RigidbodyComponent* rc)
	{
		VXM_PROFILE_FUNCTION();
		auto useSphere = [rc](Sphere& sphere) -> void { sphere.m_Body = rc; };
		auto useBox = [rc](Box& box) -> void { box.m_Body = rc; };
		auto usePlane = [rc](Plane& plane) -> void { plane.m_Body = rc; };

		std::visit(overloads{useBox, useSphere, usePlane}, m_Collider);
	}
} // namespace Voxymore::Core
