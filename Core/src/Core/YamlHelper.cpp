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

	YAML::Emitter &Core::operator<<(YAML::Emitter &out, const UUID &v)
	{
		if(v.size() == 1) {
			out << v.at(0);
			return out;
		} else {
			out << YAML::Flow;
			out << YAML::BeginSeq;
			for (const auto& val : v) out << val;
			out << YAML::EndSeq;
		}
		return out;
	}
}
