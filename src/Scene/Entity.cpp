//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Components/Components.hpp"

namespace Voxymore::Core
{
	UUID Entity::id() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(HasComponent<IDComponent>(), "The entity ID: {0} must have an ID Component.", static_cast<uint32_t>(m_EntityID));
		return GetComponent<IDComponent>();
	}
	UUID Entity::scene_id() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Scene != nullptr, "The scene is not valid.");
		if(!m_Scene) return 0;
		return m_Scene->id();
	}
	bool Entity::IsValid() const {
		VXM_PROFILE_FUNCTION();
		return m_EntityID != entt::null;
	}

	bool Entity::IsActive() const
	{
		VXM_PROFILE_FUNCTION();
		return !HasComponent<DisableComponent>();
	}

	void Entity::SetActive(bool enable)
	{
		VXM_PROFILE_FUNCTION();
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

	EntityField::EntityField(UUID entityId,UUID sceneId) : EntityId(entityId), SceneId(sceneId)
	{}

	EntityField::EntityField(Entity entity) : EntityId(entity.id()), SceneId(entity.scene_id())
	{}

	Entity EntityField::GetEntity(Scene &scene)
	{
		VXM_CORE_ASSERT(scene.id() == SceneId, "The right scene wasn't given.");
		return scene.GetEntity(EntityId);
	}
	bool EntityField::Valid()
	{
		return EntityId != 0 || SceneId != 0;
	}
}