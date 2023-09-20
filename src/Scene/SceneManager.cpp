//
// Created by ianpo on 20/09/2023.
//

#include "Voxymore/Scene/SceneManager.hpp"

#include "Voxymore/Scene/SceneSerializer.hpp"

namespace Voxymore::Core
{
	std::unordered_map<std::string, Ref<Scene>> SceneManager::m_Scenes;

	Ref<Scene> SceneManager::CreateScene(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!m_Scenes.contains(name), "The scene {0} already exist.", name);
		Ref<Scene> scene = CreateRef<Scene>(name);
		m_Scenes[name] = scene;
		return scene;
	}

	Ref<Scene> SceneManager::CreateScene(const std::filesystem::path& filepath)
	{
		VXM_PROFILE_FUNCTION();
		Ref<Scene> scene = CreateRef<Scene>();
		SceneSerializer serializer(scene);
		serializer.Deserialize(filepath.string());
		const std::string& sceneName = scene->GetName();
		VXM_CORE_ASSERT(!m_Scenes.contains(sceneName), "The scene {0} already exist.", sceneName);
		m_Scenes[sceneName] = scene;
		return scene;
	}

	Ref<Scene> SceneManager::GetScene(const std::string &name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Scenes.contains(name), "The scene {0} don't exist.", name);
		return m_Scenes[name];
	}

	Ref<Scene> SceneManager::DeleteScene(const std::string &name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Scenes.contains(name), "The scene {0} don't exist.", name);
		m_Scenes.erase(name);
	}
} // namespace Voxymore::Core
