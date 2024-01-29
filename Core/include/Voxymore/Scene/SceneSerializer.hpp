//
// Created by ianpo on 29/08/2023.
//

#pragma once

#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "yaml-cpp/emitter.h"
#include <optional>

namespace Voxymore::Core
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		SceneSerializer(Scene* scene);
		~SceneSerializer();

		bool Serialize(const Path& filePath) const;
		bool Serialize(const std::filesystem::path& filePath) const;
		// TODO: Add 'SerializeRuntime' using Voxymore::Core::Path.
		bool SerializeRuntime(const std::filesystem::path& filePath) const;

		bool Deserialize(const Path& filePath, bool deserializeId = true);
		bool Deserialize(const std::filesystem::path& filePath, bool deserializeId = true);
		// TODO: Add 'DeserializeRuntime' using Voxymore::Core::Path.
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

