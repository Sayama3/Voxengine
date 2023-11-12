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
		SceneSerializer(Scene* scene);
		~SceneSerializer();

		bool Serialize(const std::filesystem::path& filePath) const;
		bool SerializeRuntime(const std::filesystem::path& filePath) const;

		bool Deserialize(const std::filesystem::path& filePath, bool deserializeId = true);
		bool DeserializeRuntime(const std::filesystem::path& filePath, bool deserializeId = true);
		static std::optional<UUID> GetSceneID(const std::filesystem::path& filePath);
	public:
		void ChangeSceneTarget(const Ref<Scene>& scene);
		void ChangeSceneTarget(Scene* scene);
	private:
		static void SerializeEntity(YAML::Emitter& emitter, Entity entity);
	private:
		Scene& GetScene();
		const Scene& GetScene() const;
		Ref<Scene> m_RefScene = nullptr;
		Scene* m_ScenePtr = nullptr;
	};

} // namespace Voxymore::Core

