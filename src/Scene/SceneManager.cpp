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

	Ref<Scene> SceneManager::CreateScene(const std::filesystem::path& filepath, uint32_t width, uint32_t height)
	{
		VXM_PROFILE_FUNCTION();
		Ref<Scene> scene = CreateRef<Scene>();
		scene->SetViewportSize(width, height);
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

	void SceneManager::AddScene(const std::string& name, Ref<Scene> scene)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!s_Scenes.contains(name), "The scene {0} exist.", name);
		s_Scenes[name] = scene;
	}

	void SceneManager::AddScene(Ref<Scene> scene)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!s_Scenes.contains(scene->GetName()), "The scene {0} exist.", scene->GetName());
		s_Scenes[scene->GetName()] = scene;
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

	bool SceneManager::HasScene(const std::string &name)
	{
		return s_Scenes.contains(name);
	}
} // namespace Voxymore::Core
