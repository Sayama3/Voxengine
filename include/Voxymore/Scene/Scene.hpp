//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include <entt/entt.hpp>
#include "Voxymore/Core/TimeStep.hpp"

namespace Voxymore::Core
{
	class Entity;

    class Scene
    {
		friend Entity;
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
