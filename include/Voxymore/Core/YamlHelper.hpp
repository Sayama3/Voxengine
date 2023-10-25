//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include <yaml-cpp/yaml.h>
#include "Voxymore/Core/Math.hpp"

#define KEY(x) YAML::Key << x
#define VAL(x) YAML::Value << x
#define KEYVAL(x, y) KEY(x) << VAL(y)

namespace Voxymore::Core
{
	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec2& v);
	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& v);
	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& v);
	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::quat& v);
}