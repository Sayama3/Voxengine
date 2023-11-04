//
// Created by ianpo on 20/09/2023.
//

#pragma once

#include <unordered_map>
#include <filesystem>
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Scene/Scene.hpp"

namespace Voxymore::Core
{

	class SceneManager
	{
	public:
		static Ref<Scene> CreateScene(const std::string& name);
		static Ref<Scene> CreateScene(const std::filesystem::path& filepath, uint32_t width, uint32_t height);

		static Ref<Scene> GetScene(const std::string& name);
		static void DeleteScene(const std::string& name);

		static void AddScene(Ref<Scene> scene);
		static void AddScene(const std::string& name, Ref<Scene> scene);

		static bool HasScene(const std::string& name);

		static std::vector<std::string> GetSceneNames();
	private:
		static std::unordered_map<std::string, Ref<Scene>> s_Scenes;
	};

} // namespace Voxymore::Core

