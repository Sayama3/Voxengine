//
// Created by ianpo on 15/10/2023.
//

#pragma once

#include "Voxymore/Core/YamlHelper.hpp"

namespace Voxymore::Core
{
	class Serializable
	{
	public:
		virtual void Deserialize(YAML::Node& node) = 0;
		virtual void Serialize(YAML::Emitter& emitter) const = 0;
	};
}