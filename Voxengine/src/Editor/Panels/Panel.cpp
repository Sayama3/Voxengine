//
// Created by ianpo on 17/02/2024.
//

#include "Voxymore/Editor/Panels/Panel.hpp"
#include "Voxymore/Math/Math.hpp"

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
	BasePanel::PanelHandle BasePanel::s_BaseId = 0;
	BasePanel::BasePanel() : m_ID(s_BaseId++) {}
	BasePanel::BasePanel(PanelHandle id) : m_ID(id) {}

	void BasePanel::OnImGuizmo(const float* viewMatrix, const float* projectionMatrix)
	{

	}

	void BasePanel::BeginPanel()
	{
		std::string str = GetName() + "##" + Math::ToString(static_cast<uint64_t>(GetHandle()));
		ImGui::PushID(str.c_str());
		ImGui::Begin(str.c_str(), &m_Open);
	}

	void BasePanel::EndPanel()
	{
		ImGui::End();
		ImGui::PopID();
	}
}