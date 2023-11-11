//
// Created by ianpo on 29/08/2023.
//

#pragma once

#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "yaml-cpp/emitter.h"

namespace Voxymore::Core
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		~SceneSerializer();

		bool Serialize(const std::string& filePath);
		bool SerializeRuntime(const std::string& filePath);

		bool Deserialize(const std::string& filePath);
		bool DeserializeRuntime(const std::string& filePath);
		static std::optional<UUID> GetSceneID(const std::string& filePath);
	private:
		static void SerializeEntity(YAML::Emitter& emitter, Entity entity);
	private:
		Ref<Scene> m_Scene;
	};

} // namespace Voxymore::Core

