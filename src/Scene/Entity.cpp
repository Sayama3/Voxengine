//
// Created by ianpo on 24/08/2023.
//

#include "Voxymore/Scene/Entity.hpp"

namespace Voxymore::Core
{
	Entity::Entity(entt::entity entityID, Scene* scene) : m_EntityID(static_cast<uint32_t>(entityID)), m_Scene(scene)
	{
	}

	Entity::Entity(uint32_t entityID, Scene* scene) : m_EntityID(entityID), m_Scene(scene)
	{
	}

	template<typename T>
	bool Entity::HasComponent() const
	{
		m_Scene->m_Registry.any_of<T>(m_EntityID);
	}

	template<typename T>
	T& Entity::GetComponent()
	{
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component.", m_EntityID);
		return m_Scene->m_Registry.get<T>(m_EntityID);
	}

	template<typename T>
	void Entity::RemoveComponent()
	{
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component.", m_EntityID);
		m_Scene->m_Registry.remove<T>(m_EntityID);
	}

	template<typename T,  typename... Args>
	T& Entity::AddComponent(Args&&... args)
	{
		VXM_CORE_ASSERT(!HasComponent<T>(), "The entity ID: {0} already have the component.", m_EntityID);
		return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
	}
} // namespace Voxymore::Core
