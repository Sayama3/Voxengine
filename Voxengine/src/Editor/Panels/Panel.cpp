//
// Created by ianpo on 17/02/2024.
//

#include "Voxymore/Editor/Panels/Panel.hpp"
#include "Voxymore/Math/Math.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
	BasePanel::BasePanel() : m_ID(s_BaseId++) {}
	BasePanel::BasePanel(PanelHandle id) : m_ID(id) {}

	void BasePanel::OnImGuizmo(const float* viewMatrix, const float* projectionMatrix)
	{

	}

	void BasePanel::BeginPanel()
	{
		VXM_PROFILE_FUNCTION();
		std::string str = GetImGuiName();
		ImGui::PushID(str.c_str());
		ImGui::Begin(str.c_str(), &m_Open);
	}

	void BasePanel::EndPanel()
	{
		VXM_PROFILE_FUNCTION();
		ImGui::End();
		ImGui::PopID();
	}
}