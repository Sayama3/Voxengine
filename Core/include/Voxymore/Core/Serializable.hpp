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
		inline virtual void Deserialize(YAML::Node& node) {}
		inline virtual void Serialize(YAML::Emitter& emitter) const {}
		inline virtual bool OnImGui() {return false;}
	};
}