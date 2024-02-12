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
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec<L, T, Q> &v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for (int i = 0; i < L; ++i) {
			out << v[i];
		}
		out << YAML::EndSeq;
		return out;
	}

	template<typename T, glm::qualifier Q>
	inline YAML::Emitter &operator<<(YAML::Emitter &out, const glm::qua<T,Q> &v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	inline YAML::Emitter &operator<<(YAML::Emitter &out, const glm::mat<C,R,T,Q> &v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for (int x = 0; x < C; ++x) {
			for (int y = 0; y < R; ++y) {
				out << v[x][y];
			}
		}
		out << YAML::EndSeq;
		return out;
	}
	YAML::Emitter &operator<<(YAML::Emitter &out, const std::filesystem::path &v);
}
namespace YAML
{
	template<glm::length_t L, typename T, glm::qualifier Q>
	struct convert<glm::vec<L, T, Q>>
	{
		inline static Node encode(const glm::vec<L, T, Q>& rhs)
		{
			Node node;
			for (int i = 0; i < L; ++i) {
				node.push_back(rhs[i]);
			}
			return node;
		}
		inline static bool decode(const Node& node, glm::vec<L, T, Q>& rhs)
		{
			if(!node.IsSequence() || node.size() != L) return false;
			for (int i = 0; i < L; ++i) {
				rhs[i] = node[i].as<T>();
			}
			return true;
		}
	};

	template<typename T, glm::qualifier Q>
	struct convert<glm::qua<T,Q>>
	{
		inline static Node encode(const glm::qua<T,Q>& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		inline static bool decode(const Node& node, glm::qua<T,Q>& rhs)
		{
			if(!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	struct convert<glm::mat<C,R,T,Q>>
	{
		inline static Node encode(const glm::mat<C,R,T,Q>& rhs)
		{
			Node node;
			for (int x = 0; x < C; ++x) {
				for (int y = 0; y < R; ++y) {
					node.push_back(rhs[x][y]);
				}
			}
			return node;
		}
		inline static bool decode(const Node& node, glm::mat<C,R,T,Q>& rhs)
		{
			if(!node.IsSequence() || node.size() != C*R) return false;
			for (int x = 0; x < C; ++x) {
				for (int y = 0; y < R; ++y) {
					rhs[x][y] = node[x * R + y].as<T>();
				}
			}
			return true;
		}
	};
}