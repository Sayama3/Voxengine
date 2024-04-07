//
// Created by ianpo on 17/02/2024.
//

#include "Voxymore/Editor/Panels/ShaderPanel.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
	void ShaderPanel::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		ImGui::Checkbox("Auto Reload", &m_AutoReload);

		// Reloading one by one
		auto library = ShaderLibrary::GetInstance();
		for (auto &&[name, shader]: library) {
			std::string buttonName = "Reload " + name;
			if (ImGui::Button(buttonName.c_str()) || (m_AutoReload && shader->ShouldReload()))
			{
				shader->Reload();
			}
		}
	}
} // namespace Voxymore::Editor
