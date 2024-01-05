//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/ImGUI/ImGuiLib.hpp"
#include "Voxymore/ImGUI/ImGUILayer.hpp"
#include "Voxymore/Core/Application.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#ifdef VXM_DOUBLE
#define IMGUI_SCALAR_TYPE ImGuiDataType_Double
#else //FLOAT
#define IMGUI_SCALAR_TYPE ImGuiDataType_Float
#endif


namespace Voxymore
{
	namespace Core
	{
		bool ImGuiLib::DrawVec3ControlAdvanced(const std::string& label, Vec3& values, float step, Real min, Real max, const std::string& format, float resetValue, float columnWidth) {
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
			changed |= ImGui::DragScalar("##X", IMGUI_SCALAR_TYPE, &values.x, step, &min, &max, format.c_str());
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
			changed |= ImGui::DragScalar("##Y", IMGUI_SCALAR_TYPE, &values.y, step, &min, &max, format.c_str());
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
			changed |= ImGui::DragScalar("##Z", IMGUI_SCALAR_TYPE, &values.z, step, &min, &max, format.c_str());
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();

			// ==================== END ====================
			ImGui::Columns(1);
			ImGui::PopID();
			return changed;
		}

		bool ImGuiLib::DrawVec2ControlAdvanced(const std::string& label, Vec2& values, float step, Real min, Real max, const std::string& format, float resetValue, float columnWidth) {
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
			changed |= ImGui::DragScalar("##X", IMGUI_SCALAR_TYPE, &values.x, step, &min, &max, format.c_str());
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
			changed |= ImGui::DragScalar("##Y", IMGUI_SCALAR_TYPE, &values.y, step, &min, &max, format.c_str());
			ImGui::PopItemWidth();

			// ==================== END ====================
			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
			return changed;
		}

		bool ImGuiLib::DragReal(const std::string &label, Real *v, float v_speed, Real v_min, Real v_max, const char *format, ImGuiSliderFlags flags)
		{
			return ImGui::DragScalar(label.c_str(), IMGUI_SCALAR_TYPE, v, v_speed, &v_min, &v_max, format, flags);
		}

		bool ImGuiLib::DragReal2(const std::string &label, Real *v, float v_speed, Real v_min, Real v_max, const char *format, ImGuiSliderFlags flags)
		{
			return ImGui::DragScalarN(label.c_str(), IMGUI_SCALAR_TYPE, v, 2, v_speed, &v_min, &v_max, format, flags);
		}

		bool ImGuiLib::DragReal3(const std::string &label, Real *v, float v_speed, Real v_min, Real v_max, const char *format, ImGuiSliderFlags flags)
		{
			return ImGui::DragScalarN(label.c_str(), IMGUI_SCALAR_TYPE, v, 3, v_speed, &v_min, &v_max, format, flags);
		}

		bool ImGuiLib::DragReal4(const std::string &label, Real *v, float v_speed, Real v_min, Real v_max, const char *format, ImGuiSliderFlags flags)
		{
			return ImGui::DragScalarN(label.c_str(), IMGUI_SCALAR_TYPE, v, 4, v_speed, &v_min, &v_max, format, flags);
		}

		bool ImGuiLib::DragRealRange2(const std::string &label, Real *v_current_min, Real *v_current_max, float v_speed, Real v_min, Real v_max, const char *format, const char *format_max, ImGuiSliderFlags flags)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			ImGui::PushID(label.c_str());
			ImGui::BeginGroup();
			ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

			float min_min = (v_min >= v_max) ? -FLT_MAX : v_min;
			float min_max = (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max);
			ImGuiSliderFlags min_flags = flags | ((min_min == min_max) ? ImGuiSliderFlags_ReadOnly : 0);
			bool value_changed = ImGui::DragScalar("##min", IMGUI_SCALAR_TYPE, v_current_min, v_speed, &min_min, &min_max, format, min_flags);
			ImGui::PopItemWidth();
			ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);

			float max_min = (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min);
			float max_max = (v_min >= v_max) ? FLT_MAX : v_max;
			ImGuiSliderFlags max_flags = flags | ((max_min == max_max) ? ImGuiSliderFlags_ReadOnly : 0);
			value_changed |= ImGui::DragScalar("##max", IMGUI_SCALAR_TYPE, v_current_max, v_speed, &max_min, &max_max, format_max ? format_max : format, max_flags);
			ImGui::PopItemWidth();
			ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);

			ImGui::TextEx(label.c_str(), ImGui::FindRenderedTextEnd(label.c_str()));
			ImGui::EndGroup();
			ImGui::PopID();

			return value_changed;
		}

	}// namespace Core
}// namespace Voxymore