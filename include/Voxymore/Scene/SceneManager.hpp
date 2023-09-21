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
		static Ref<Scene> CreateScene(const std::filesystem::path& filepath);

		static Ref<Scene> GetScene(const std::string& name);
		static void DeleteScene(const std::string& name);
	private:
		static std::unordered_map<std::string, Ref<Scene>> m_Scenes;
	};

} // namespace Voxymore::Core

