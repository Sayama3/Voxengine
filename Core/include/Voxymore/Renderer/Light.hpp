//
// Created by ianpo on 16/02/2024.
//

#pragma once

#include <Voxymore/Core/Math.hpp>

namespace Voxymore::Core
{
	enum class LightType : int
	{
		Directional,
		Point,
		Spot
	};

	inline std::vector<std::string> GetLightTypeName() { return {"Directional", "Point", "Spot"};}

	struct Light
	{
		inline Light() = default;
		inline ~Light() = default;
		inline Light(glm::vec4 color, glm::vec3 position, glm::vec3 direction, float range, float intensity, float cutoff, LightType type) : Color(color), Position(glm::vec4(position, 1)), Direction(glm::vec4(direction, 0)), Range(range), Intensity(intensity), Cutoff(cutoff), Type((int)type) {}

		glm::vec4 Color;
		glm::vec4 Position;
		glm::vec4 Direction;
		float Range;
		float Intensity;
		float Cutoff;
		int Type;
	};

}// namespace Voxymore
