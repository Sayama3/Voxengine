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
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(entt::entity entityID, Scene* scene);

		inline bool IsValid() const {return m_EntityID != entt::null;}

		template<typename T>
		bool HasComponent() const;

		template<typename T>
		T& GetComponent();

		template<typename T,  typename... Args>
		T& AddComponent(Args &&...args);

		template<typename T>
		void RemoveComponent();

		operator bool() const { return IsValid(); }
		operator entt::entity() const { return m_EntityID; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityID); }

		bool operator==(const Entity& other) const
		{
			return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	};
} // namespace Voxymore::Core

