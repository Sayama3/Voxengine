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
				auto library = ShaderLibrary::GetInstance();
				for (auto &&[name, shader]: library) {
					std::string buttonName = "Reload " + name;
					if (ImGui::Button(buttonName.c_str())) {
						shader->Reload();
					}
				}
			}
			ImGui::End();
		}
	}
} // namespace Voxymore::Editor
