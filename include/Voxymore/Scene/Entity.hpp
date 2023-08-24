//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Scene/Scene.hpp"
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
		inline Entity(entt::entity entityID, Scene* scene) : m_EntityID(entityID), m_Scene(scene)
		{
		}

		inline bool IsValid() const {return m_EntityID != entt::null;}

		template<typename T>
		inline bool HasComponent() const
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityID);
		}

		template<typename T>
		inline T& GetComponent()
		{
			VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component.", static_cast<uint32_t>(m_EntityID));
			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T,  typename... Args>
		inline T& AddComponent(Args &&...args)
		{
			VXM_CORE_ASSERT(!HasComponent<T>(), "The entity ID: {0} already have the component.", static_cast<uint32_t>(m_EntityID));
			return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component.", static_cast<uint32_t>(m_EntityID));
			m_Scene->m_Registry.remove<T>(m_EntityID);
		}

		inline operator bool() const { return IsValid(); }
		inline operator entt::entity() const { return m_EntityID; }
		inline operator uint32_t() const { return static_cast<uint32_t>(m_EntityID); }

		inline bool operator==(const Entity& other) const
		{
			return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene;
		}

		inline bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	};
} // namespace Voxymore::Core

