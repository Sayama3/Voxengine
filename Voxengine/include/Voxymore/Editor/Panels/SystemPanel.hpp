#pragma once

#include "Voxymore/Editor/Panels/Panel.hpp"
#include "Voxymore/Voxymore.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
    class SystemPanel : public Panel<SystemPanel>
	{
private:
        static std::vector<std::string> m_SceneNames;
        static std::vector<UUID> m_SceneIds;
        void DrawSystem(const std::string& systemName);
public:
		VXM_IMPLEMENT_PANEL("Systems");
		virtual void OnImGuiRender() override;
    };
} // namespace Voxymore::Editor
