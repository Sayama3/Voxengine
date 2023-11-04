//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include <yaml-cpp/yaml.h>
#include "Voxymore/Core/Math.hpp"
#include <filesystem>

#define KEY(x) YAML::Key << x
#define VAL(x) YAML::Value << x
#define KEYVAL(x, y) KEY(x) << VAL(y)
namespace Voxymore::Core
{
	YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec2 &v);
	YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 &v);
	YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 &v);
	YAML::Emitter &operator<<(YAML::Emitter &out, const glm::quat &v);
	YAML::Emitter &operator<<(YAML::Emitter &out, const std::filesystem::path &v);
}
namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		inline static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		inline static bool decode(const Node& node, glm::vec2& rhs)
		{
			if(!node.IsSequence() || node.size() != 2) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3>
	{
		inline static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		inline static bool decode(const Node& node, glm::vec3& rhs)
		{
			if(!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		inline static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		inline static bool decode(const Node& node, glm::vec4& rhs)
		{
			if(!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::quat>
	{
		inline static Node encode(const glm::quat& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		inline static bool decode(const Node& node, glm::quat& rhs)
		{
			if(!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}