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
		static bool DrawVec3ControlAdvanced(const std::string& label, glm::vec3& values, float step = 0.1f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f", float resetValue = 0.0f, float columnWidth = 100.0f);
		static bool DrawVec2ControlAdvanced(const std::string& label, glm::vec2& values, float step = 0.1f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f", float resetValue = 0.0f, float columnWidth = 100.0f);
		inline static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
		{
			return DrawVec3ControlAdvanced(label, values, 0.1f, 0.0f, 0.0f, "%.3f", resetValue, columnWidth);
		}
		inline static bool DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f)
		{
			return DrawVec2ControlAdvanced(label, values, 0.1f, 0.0f, 0.0f, "%.3f", resetValue, columnWidth);
		}
	};

} // namespace Voxymore::Core

