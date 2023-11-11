//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/Components.hpp"

namespace Voxymore::Core
{
	UUID Entity::GetUUID() const
	{
		VXM_CORE_ASSERT(HasComponent<IDComponent>(), "The entity ID: {0} must have an ID Component.", static_cast<uint32_t>(m_EntityID));
		return GetComponent<IDComponent>();
	}
}