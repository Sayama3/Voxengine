//
// Created by ianpo on 15/10/2023.
//

#include "Voxymore/Core/YamlHelper.hpp"
namespace Voxymore::Core
{

	YAML::Emitter &operator<<(YAML::Emitter &out, const std::filesystem::path &v)
	{
		out << v.string();
		return out;
	}
}
