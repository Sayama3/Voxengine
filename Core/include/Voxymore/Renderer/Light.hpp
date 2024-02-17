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
		inline Light(glm::vec4 color, glm::vec3 position, glm::vec3 direction, float range, float intensity, LightType type) : Color(color), Position(position), Direction(direction), Range(range), Intensity(intensity), Type(type) {}

		glm::vec4 Color;
		glm::vec3 Position;
		glm::vec3 Direction;
		float Range;
		float Intensity;
		LightType Type;
	};

}// namespace Voxymore
