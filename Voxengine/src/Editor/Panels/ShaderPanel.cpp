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
		if(m_Open) {
			if (ImGui::Begin("Shader Panel", &m_Open)) {
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
			ImGui::End();
		}
	}
} // namespace Voxymore::Editor
