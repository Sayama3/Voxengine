//
// Created by ianpo on 27/01/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Scene/Entity.forward.hpp"
#include "Voxymore/Scene/Scene.forward.hpp"
#include <entt/entt.hpp>

namespace Voxymore::Core
{
	class Entity
	{
	private:
		entt::entity m_EntityID{entt::null};
		Scene* m_Scene = nullptr;
	public:
		inline Entity() = default;
		inline Entity(const Entity&) = default;
		inline Entity(entt::entity entityID, Scene* scene) : m_EntityID(entityID), m_Scene(scene) {}

		template<typename T>
		bool HasComponent() const;

		template<typename T>
		T& GetComponent();

		template<typename T>
		const T& GetComponent() const;

		template<typename T,  typename... Args>
		T& AddComponent(Args &&...args);

		template<typename T>
		void AddEmptyComponent();

		template<typename T,  typename... Args>
		T& GetOrAddComponent(Args &&...args);

		template<typename T>
		void RemoveComponent();



		inline operator bool() const { return IsValid(); }
		operator entt::entity() const;
		operator uint32_t() const;

		bool operator==(const Entity& other) const;

		bool operator!=(const Entity& other) const;


		bool IsValid() const;
		bool IsActive() const;
		void SetActive(bool enable);
		[[deprecated("Use id()")]]
		inline UUID GetUUID() const {return id();}
		UUID id() const;
		UUID scene_id() const;
	};

	struct EntityField
	{
		EntityField(UUID entityId, UUID sceneId);
		explicit EntityField(Entity entity);
		inline EntityField() = default;
		inline ~EntityField() = default;
		UUID EntityId;
		UUID SceneId;

		bool Valid();
		Entity GetEntity(Scene& scene);
	};
}

namespace std
{
	template<>
	struct hash<Voxymore::Core::Entity>
	{
		std::size_t operator()(const Voxymore::Core::Entity& e) const
		{
			const std::size_t id_hash = std::hash<Voxymore::Core::UUID>{}(e.id());
			const std::size_t scene_id_hash = std::hash<Voxymore::Core::UUID>{}(e.scene_id());
			return id_hash ^ (scene_id_hash << 1);
		}
	};
} // namespace std