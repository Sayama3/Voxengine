//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include <entt/entt.hpp>

namespace Voxymore::Core {

    class Scene
    {
    public:
        Scene();
        ~Scene();
    private:
        entt::registry m_Registry;
    };

} // Voxymore
// Core
