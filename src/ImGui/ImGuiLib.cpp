//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/ImGUI/ImGuiLib.hpp"
#include "Voxymore/ImGUI/ImGUILayer.hpp"
#include "Voxymore/Core/Application.hpp"
#include "imgui_internal.h"

namespace Voxymore
{
	namespace Core
	{
		bool ImGuiLib::DrawVec3ControlAdvanced(const std::string& label, glm::vec3& values, float step, float min, float max, const std::string& format, float resetValue, float columnWidth) {
			VXM_PROFILE_FUNCTION();
			// ==================== START ====================
			bool changed = false;
			ImFont* boldFont = Application::Get().GetImGuiLayer()->GetFont(FontType::Bold);
			ImGui::PushID(label.c_str());
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("%s", label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);


			// ==================== X ====================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.3f, 0.25f, 1.0f));
			ImGui::PushFont(boldFont);
			if(ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
				changed |= true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			changed |= ImGui::DragFloat("##X", &values.x, step, min, max, format.c_str());
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// ==================== Y ====================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
				changed |= true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			changed |= ImGui::DragFloat("##Y", &values.y, step, min, max, format.c_str());
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// ==================== Z ====================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.45f, 1.0f, 1.0f));
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Z", buttonSize))
			{
				values.z = resetValue;
				changed |= true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			changed |= ImGui::DragFloat("##Z", &values.z, step, min, max, format.c_str());
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();

			// ==================== END ====================
			ImGui::Columns(1);
			ImGui::PopID();
			return changed;
		}
		bool ImGuiLib::DrawVec2ControlAdvanced(const std::string& label, glm::vec2& values, float step, float min, float max, const std::string& format, float resetValue, float columnWidth) {
			VXM_PROFILE_FUNCTION();
			// ==================== START ====================
			bool changed = false;
			ImFont* boldFont = Application::Get().GetImGuiLayer()->GetFont(FontType::Bold);
			ImGui::PushID(label.c_str());
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("%s", label.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);

			// ==================== X ====================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.3f, 0.25f, 1.0f));
			ImGui::PushFont(boldFont);
			if(ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
				changed |= true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			changed |= ImGui::DragFloat("##X", &values.x, step, min, max, format.c_str());
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// ==================== Y ====================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
				changed |= true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			changed |= ImGui::DragFloat("##Y", &values.y, step, min, max, format.c_str());
			ImGui::PopItemWidth();

			// ==================== END ====================
			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
			return changed;
		}

	}// namespace Core
}// namespace Voxymore