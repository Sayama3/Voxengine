//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include <entt/entt.hpp>
#include "Voxymore/Core/TimeStep.hpp"

namespace Voxymore::Core
{

    class Scene
    {
    public:
        Scene();
        ~Scene();

		entt::entity CreateEntity();
		entt::registry& Reg();
		void OnUpdate(TimeStep ts);
    private:
        entt::registry m_Registry;
    };

} // Voxymore
// Core
