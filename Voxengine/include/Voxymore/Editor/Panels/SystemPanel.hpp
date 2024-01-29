#pragma once

#include "Voxymore/Voxymore.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
    class SystemPanel
    {
private:
        std::vector<std::string> m_SceneNames;
        std::vector<UUID> m_SceneIds;
        void DrawSystem(const std::string& systemName);
public:
        void OnImGuiRender();
    };
} // namespace Voxymore::Editor
