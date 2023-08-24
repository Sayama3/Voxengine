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
    private:
        entt::registry m_Registry;
    };

} // Voxymore
// Core
