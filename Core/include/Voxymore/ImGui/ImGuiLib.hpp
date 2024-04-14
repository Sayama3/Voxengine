//
// Created by ianpo on 11/11/2023.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Assets/AssetField.hpp"
#include <iterator>
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

		template<typename T>
		static bool DrawVector(const char* label, std::vector<T>* list, std::function<bool(const char*, T*)> drawFunc, const T& defaultValue, uint64_t minElement = 0, uint64_t maxElement = 0);
		template<typename T>
		inline static bool DrawVector(const char* label, std::vector<T>* list, std::function<bool(const char*, T*)> drawFunc, uint64_t minElement = 0, uint64_t maxElement = 0) { return DrawVector(label, list, drawFunc, T(), minElement, maxElement); }

		// fetched from the misc cpp of ImGui
		// ImGui::InputText() with std::string
		// Because text input needs dynamic resizing, we need to setup a callback to grow the capacity
		static bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
		static bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
		static bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
	};

	template<typename T>
	bool ImGuiLib::DrawVector(const char *label, std::vector<T>* list, std::function<bool(const char*, T*)> drawFunc, const T& defaultValue, uint64_t minElement, uint64_t maxElement)
	{
		bool changed = false;
		ImGui::PushID(label);
		const ImGuiTreeNodeFlags treeNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_AllowItemOverlap |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_Framed;

		bool hasMinMax = maxElement && maxElement>=minElement;
		ImVec2 contentAvailable = ImGui::GetContentRegionMax();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		float buttonWidth = lineHeight * 4;
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", label);
		ImGui::PopStyleVar();

		// Add missing one
		for (uint64_t i = list->size(); i < minElement; ++i) {
			list->push_back(defaultValue);
		}

		ImGui::SameLine(contentAvailable.x - buttonWidth + 4);
		ImGui::BeginDisabled(hasMinMax && list->size() >= maxElement);
		if(ImGui::Button("+", ImVec2(buttonWidth, lineHeight)))
		{
			auto inserter = std::back_inserter<std::vector<T>>(*list);
			inserter = defaultValue;
			changed = true;
		}
		ImGui::EndDisabled();

		if(open) {
			const char *name = "##Element";
			for (int i = 0; i < list->size(); ++i) {
				ImGui::PushID(i);
				ImGui::Text("%d", i);
				ImGui::SameLine();
				changed |= drawFunc(name, (T *) &(list->at(i)));
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
				ImGui::BeginDisabled(hasMinMax && list->size() <= minElement);
				if (ImGui::Button(" X ")) {
					list->erase(list->begin() + i);
					--i;
				}
				ImGui::EndDisabled();
				ImGui::PopStyleColor();
				ImGui::PopID();
			}
			ImGui::Separator();
			ImGui::TreePop();
		}

		// erase the surplus
		if (hasMinMax &&list->size() > maxElement) {
			list->erase(list->begin() + maxElement, list->begin() + list->size());
		}

		ImGui::PopID();
		return changed;
	}
} // namespace Voxymore::Core
