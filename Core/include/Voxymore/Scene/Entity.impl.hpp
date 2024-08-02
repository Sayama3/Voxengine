//
// Created by ianpo on 27/01/2024.
//

#pragma once

#include "Voxymore/Scene/Entity.decl.hpp"
#include "Voxymore/Scene/Scene.decl.hpp"

namespace Voxymore::Core
{
	template<typename T>
	inline bool Entity::HasComponent() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Scene->m_Registry.any_of<T>(m_EntityID);
	}

	template<typename T>
	inline T& Entity::GetComponent()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component '{1}'.", static_cast<uint32_t>(m_EntityID), typeid(T).name());
		return m_Scene->m_Registry.get<T>(m_EntityID);
	}

	template<typename T>
	inline const T& Entity::GetComponent() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component '{1}'.", static_cast<uint32_t>(m_EntityID), typeid(T).name());
		return m_Scene->m_Registry.get<T>(m_EntityID);
	}

	template<typename T,  typename... Args>
	inline T& Entity::AddComponent(Args &&...args)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!HasComponent<T>(), "The entity ID: {0} already have the component '{1}'.", static_cast<uint32_t>(m_EntityID), typeid(T).name());
		T& component = m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		// Using the operator in case later on we change this to take our Entity
		m_Scene->OnComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T>
	inline void Entity::AddEmptyComponent()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!HasComponent<T>(), "The entity ID: {0} already have the component '{1}'.", static_cast<uint32_t>(m_EntityID), typeid(T).name());
		m_Scene->m_Registry.emplace<T>(m_EntityID);
		// Using the operator in case later on we change this to take our Entity
		m_Scene->OnEmptyComponentAdded<T>(*this);
	}

	template<typename T,  typename... Args>
	inline T& Entity::GetOrAddComponent(Args &&...args)
	{
		VXM_PROFILE_FUNCTION();
		if(HasComponent<T>())
		{
			return GetComponent<T>();
		}
		else
		{
			return AddComponent<T>(std::forward<Args>(args)...);
		}
	}

	template<typename T>
	inline void Entity::EnsureHasEmptyComponent()
	{
		VXM_PROFILE_FUNCTION();
		if(!HasComponent<T>())
		{
			return AddEmptyComponent<T>();
		}
	}

	template<typename T>
	inline void Entity::RemoveComponent()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component '{1}'.", static_cast<uint32_t>(m_EntityID), typeid(T).name());
		m_Scene->m_Registry.remove<T>(m_EntityID);
	}
} // namespace Voxymore::Core
