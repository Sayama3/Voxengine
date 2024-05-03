//
// Created by ianpo on 20/09/2023.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

#include <unordered_map>
#include <filesystem>

namespace Voxymore::Core
{

	class SceneManager
	{
	public:
		static Ref<Scene> CreateScene();
		static Ref<Scene> CreateScene(const std::string& name);
//		static Ref<Scene> CreateScene(const std::filesystem::path& filepath, uint32_t width, uint32_t height);

		static Ref<Scene> GetScene(UUID id);
		static void DeleteScene(UUID id);

		static void AddScene(Ref<Scene> scene);

		static bool HasScene(UUID name);

		static std::vector<std::string> GetSceneNames();
		static std::vector<UUID> GetSceneIds();

		static bool IsLoaded(const std::filesystem::path& filepath);

		static void Clear();
	private:
		static std::unordered_map<UUID, Ref<Scene>> s_Scenes;
	};

} // namespace Voxymore::Core

