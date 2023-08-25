//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include <entt/entt.hpp>
#include "Voxymore/Core/TimeStep.hpp"

// TODO: find a better way?
namespace Voxymore::Editor {
	class SceneHierarchyPanel;
}

//TODO: add a way to create/delete system (that can go onto the whole scene) instead of juste NativeScriptComponent.
namespace Voxymore::Core
{
	class Entity;

    class Scene
    {
	private:
		friend class Voxymore::Editor::SceneHierarchyPanel;
		friend class Entity;
    public:
        Scene();
        ~Scene();

		Entity CreateEntity(const std::string& name = "");

		void OnUpdate(TimeStep ts);
		void SetViewportSize(uint32_t width, uint32_t height);
    private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        entt::registry m_Registry;
    };

} // Voxymore
// Core
