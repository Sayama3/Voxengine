//
// Created by ianpo on 20/09/2023.
//

#include "Voxymore/Scene/SceneManager.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"

namespace Voxymore::Core
{
	std::unordered_map<UUID, Ref<Scene>> SceneManager::s_Scenes;

	Ref<Scene> SceneManager::CreateScene()
	{
		VXM_PROFILE_FUNCTION();
		Ref<Scene> scene = CreateRef<Scene>();
		UUID id = scene->GetID();
		VXM_CORE_ASSERT(!s_Scenes.contains(id), "The scene id '{0}' already exist.", id);
		s_Scenes[id] = scene;
		return scene;
	}
	Ref<Scene> SceneManager::CreateScene(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		Ref<Scene> scene = CreateRef<Scene>(name);
		UUID id = scene->GetID();
		VXM_CORE_ASSERT(!s_Scenes.contains(id), "The scene id '{0}' already exist.", id);
		s_Scenes[id] = scene;
		return scene;
	}

//	Ref<Scene> SceneManager::CreateScene(const std::filesystem::path& filepath, uint32_t width, uint32_t height)
//	{
//		VXM_PROFILE_FUNCTION();
//		Ref<Scene> scene = CreateRef<Scene>();
//		scene->SetViewportSize(width, height);
//		SceneSerializer serializer(scene);
//		serializer.Deserialize(filepath.string());
//		UUID id = scene->GetID();
//		if(s_Scenes.contains(id))
//		{
//			VXM_CORE_WARNING("The scene id {0} is already loaded. Not creating a new scene.");
//			return s_Scenes[id];
//		}
//		else
//		{
//			s_Scenes[id] = scene;
//			return scene;
//		}
//	}

	Ref<Scene> SceneManager::GetScene(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_Scenes.contains(id), "The scene {0} don't exist.", id);
		return s_Scenes[id];
	}

	void SceneManager::DeleteScene(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_Scenes.contains(id), "The scene {0} don't exist.", id);
		s_Scenes.erase(id);
	}

	void SceneManager::AddScene(Ref<Scene> scene)
	{
		VXM_PROFILE_FUNCTION();
		UUID id = scene->id();
		VXM_CORE_ASSERT(!s_Scenes.contains(id), "The scene {0} exist.", id);
		s_Scenes[id] = scene;
	}
	
	std::vector<std::string> SceneManager::GetSceneNames()
	{
		VXM_PROFILE_FUNCTION();
		std::vector<std::string> scenes;
		scenes.reserve(s_Scenes.size());
		for (auto& kv : s_Scenes)
		{
			scenes.push_back(kv.second->GetName());
		}
		return scenes;
	}

	std::vector<UUID> SceneManager::GetSceneIds()
	{
		VXM_PROFILE_FUNCTION();
		std::vector<UUID> scenes;
		scenes.reserve(s_Scenes.size());
		for (auto& kv : s_Scenes)
		{
			scenes.push_back(kv.first);
		}
		return scenes;
	}

	bool SceneManager::HasScene(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		return s_Scenes.contains(id);
	}

	bool SceneManager::IsLoaded(const std::filesystem::path &filepath)
	{
		VXM_PROFILE_FUNCTION();
		auto id = SceneSerializer::GetSceneID(filepath.string());
		if(!id.has_value())
		{
			VXM_CORE_WARNING("The scene '{0}' do not have an ID and is most likely not a scene...", filepath.string());
			return false;
		}
		return HasScene(id.value());
	}

	void SceneManager::Clear()
	{
		VXM_PROFILE_FUNCTION();
		s_Scenes.clear();
	}
} // namespace Voxymore::Core
