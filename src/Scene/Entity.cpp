//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Components/Components.hpp"

namespace Voxymore::Core
{
	UUID Entity::GetUUID() const
	{
		VXM_CORE_ASSERT(HasComponent<IDComponent>(), "The entity ID: {0} must have an ID Component.", static_cast<uint32_t>(m_EntityID));
		return GetComponent<IDComponent>();
	}
	bool Entity::IsValid() const {return m_EntityID != entt::null;}

	bool Entity::IsActive() const
	{
		return !HasComponent<DisableComponent>();
	}

	void Entity::SetActive(bool enable)
	{
		if(IsActive() == enable)
		{
			// Current state == desired state. Do nothing.
			return;
		}
		if(enable)
		{
			RemoveComponent<DisableComponent>();
		}
		else
		{
			AddEmptyComponent<DisableComponent>();
		}
	}

}