//
// Created by ianpo on 11/11/2023.
//

#pragma once

#include "Voxymore/Core/Math.hpp"
#include <imgui.h>

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
	};

} // namespace Voxymore::Core

