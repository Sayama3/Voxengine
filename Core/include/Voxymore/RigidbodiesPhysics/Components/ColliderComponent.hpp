//
// Created by ianpo on 15/03/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/RigidbodiesPhysics/Primitive.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/Math/BoundingSphere.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include <variant>

namespace Voxymore::Core
{

	struct ColliderComponent : public SelfAwareComponent<ColliderComponent>
	{
		VXM_IMPLEMENT_SELFAWARECOMPONENT(ColliderComponent);
	public:
		inline ColliderComponent() = default;
		inline ~ColliderComponent() = default;

		void DeserializeComponent(YAML::Node& node, Entity e);
		void SerializeComponent(YAML::Emitter& out, Entity e);
		bool OnImGuiRender(Entity e);
	public:

		template<class Collider>
		inline bool Is() const { return std::holds_alternative<Collider>(m_Collider) != nullptr; }

		template<class Collider>
		inline Collider* TryGet() { return std::get_if<Collider>(&m_Collider); }
		template<class Collider>
		inline const Collider* TryGet() const { return std::get_if<Collider>(&m_Collider); }

		template<class Collider>
		inline Collider& Get() { return std::get<Collider>(m_Collider); }
		template<class Collider>
		inline const Collider& Get() const { return std::get<Collider>(m_Collider); }

		[[nodiscard]] BoundingSphere GetBoundingSphere() const;
		[[nodiscard]] BoundingBox GetBoundingBox() const;

		void SetTransform(TransformComponent* tc);
		void SetRigidbody(RigidbodyComponent* rc);


		std::variant<Sphere, Plane, Box> m_Collider;
	};

} // namespace Voxymore::Core

