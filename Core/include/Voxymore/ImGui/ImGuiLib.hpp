//
// Created by ianpo on 11/11/2023.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Assets/AssetField.hpp"
#include <imgui.h>
#include <imgui_internal.h>

namespace Voxymore::Core
{
	class ImGuiLib
	{
	public:
		static bool DrawVec3ControlAdvanced(const std::string& label, Vec3& values, float step = 0.1f, Real min = 0, Real max = 0, const std::string& format = "%.3f", float resetValue = 0.0f, float columnWidth = 100.0f);
		static bool DrawVec2ControlAdvanced(const std::string& label, Vec2& values, float step = 0.1f, Real min = 0, Real max = 0, const std::string& format = "%.3f", float resetValue = 0.0f, float columnWidth = 100.0f);
		inline static bool DrawVec3Control(const std::string& label, Vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
		{
			return DrawVec3ControlAdvanced(label, values, 0.1f, 0, 0, "%.3f", resetValue, columnWidth);
		}
		inline static bool DrawVec2Control(const std::string& label, Vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f)
		{
			return DrawVec2ControlAdvanced(label, values, 0.1f, 0, 0, "%.3f", resetValue, columnWidth);
		}

		static bool DragReal(const std::string& label, Real* v, float v_speed = 1.0f, Real v_min = 0, Real v_max = 0, const char *format = "%.3f", ImGuiSliderFlags flags = 0);
		static bool DragReal2(const std::string& label, Real* v, float v_speed = 1.0f, Real v_min = 0, Real v_max = 0, const char *format = "%.3f", ImGuiSliderFlags flags = 0);
		static bool DragReal3(const std::string& label, Real* v, float v_speed = 1.0f, Real v_min = 0, Real v_max = 0, const char *format = "%.3f", ImGuiSliderFlags flags = 0);
		static bool DragReal4(const std::string& label, Real* v, float v_speed = 1.0f, Real v_min = 0, Real v_max = 0, const char *format = "%.3f", ImGuiSliderFlags flags = 0);

		/**
		 * NB: You likely want to specify the ImGuiSliderFlags_AlwaysClamp when using this.
		 */
		static bool DragRealRange2(const std::string& label, Real *v_current_min, Real *v_current_max, float v_speed = 1.0f, Real v_min = 0.0f, Real v_max = 0.0f, const char *format = "%.3f", const char *format_max = 0, ImGuiSliderFlags flags = 0);

		static bool InputUUID(const char* name, UUID* id, ImGuiInputTextFlags flags = 0);

		template<typename T>
		static bool DrawAssetField(const char* name, AssetField<T>* v)
		{
			VXM_PROFILE_FUNCTION();
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems) {
				return false;
			}
			bool changed = false;
			const AssetType assetType = AssetField<T>::GetStaticType();

			ImGuiContext& g = *GImGui;
			ImGui::BeginGroup();
			ImGui::PushID(name);

			// TODO: Draw a little preview

			AssetHandle handle = v->GetHandle();
			if(ImGuiLib::InputUUID("##UUID", &handle))
			{
				v->SetHandle(handle);
				changed = true;
			}


			if(ImGui::BeginDragDropTarget())
			{
				auto payloadStr = AssetTypeToPayloadID(assetType);
				VXM_CORE_ASSERT(payloadStr.size() < 32, "The payloadID is too large.");
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(payloadStr.c_str());
				if (payload != nullptr) {
					VXM_CORE_ASSERT(payload->DataSize == sizeof(AssetHandle), "The data is not an AssetHandle");
					auto payloadHandle = *((AssetHandle*)(payload->Data));
					v->SetHandle(payloadHandle);
					changed = true;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
			if(ImGui::Button("Reset")) {
				v->SetHandle(0);
				changed = true;
			}

			ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
			ImGui::TextEx(name, ImGui::FindRenderedTextEnd(name));

			ImGui::PopID();
			ImGui::EndGroup();

			return changed;
		}

		static float GetWindowFontScale();

		// fetched from the misc cpp of ImGui
		// ImGui::InputText() with std::string
		// Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
		static bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
		static bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
		static bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
	};

} // namespace Voxymore::Core

