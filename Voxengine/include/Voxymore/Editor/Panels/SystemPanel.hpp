#pragma once

#include "Voxymore/Voxymore.hpp"
#include "Voxymore/Editor/Panels/Panel.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
    class SystemPanel : public Panel
    {
private:
        std::vector<std::string> m_SceneNames;
        std::vector<UUID> m_SceneIds;
        void DrawSystem(const std::string& systemName);
public:
		virtual void OnImGuiRender() override;
    };
} // namespace Voxymore::Editor
