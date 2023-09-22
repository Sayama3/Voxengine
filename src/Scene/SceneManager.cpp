//
// Created by ianpo on 20/09/2023.
//

#include "Voxymore/Scene/SceneManager.hpp"

#include "Voxymore/Scene/SceneSerializer.hpp"

namespace Voxymore::Core
{
	std::unordered_map<std::string, Ref<Scene>> SceneManager::s_Scenes;

	Ref<Scene> SceneManager::CreateScene(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!s_Scenes.contains(name), "The scene {0} already exist.", name);
		Ref<Scene> scene = CreateRef<Scene>(name);
		s_Scenes[name] = scene;
		return scene;
	}

	Ref<Scene> SceneManager::CreateScene(const std::filesystem::path& filepath)
	{
		VXM_PROFILE_FUNCTION();
		Ref<Scene> scene = CreateRef<Scene>();
		SceneSerializer serializer(scene);
		serializer.Deserialize(filepath.string());
		const std::string& sceneName = scene->GetName();
		VXM_CORE_ASSERT(!s_Scenes.contains(sceneName), "The scene {0} already exist.", sceneName);
		s_Scenes[sceneName] = scene;
		return scene;
	}

	Ref<Scene> SceneManager::GetScene(const std::string &name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_Scenes.contains(name), "The scene {0} don't exist.", name);
		return s_Scenes[name];
	}

	void SceneManager::DeleteScene(const std::string &name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_Scenes.contains(name), "The scene {0} don't exist.", name);
		s_Scenes.erase(name);
	}

	
	std::vector<std::string> SceneManager::GetSceneNames()
	{
		VXM_PROFILE_FUNCTION();
		std::vector<std::string> scenes;
		scenes.reserve(s_Scenes.size());
		for (auto& kv : s_Scenes)
		{
			scenes.push_back(kv.first);
		}
		return scenes;
	}
} // namespace Voxymore::Core
