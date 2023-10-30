//
// Created by ianpo on 21/09/2023.
//

#include <utility>

#include "Voxymore/Scene/GameplaySystem.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Project/Project.hpp"

namespace Voxymore::Core
{
	SystemManager* SystemManager::s_SystemManager = nullptr;

	SystemManager &SystemManager::GetInstance()
	{
		if(s_SystemManager == nullptr) s_SystemManager = new SystemManager();
		return *s_SystemManager;
	}

	Path SystemManager::GetPath(const std::string& name)
	{
		return {FileSource::System, name};
	}
	void SystemManager::AddSystem(std::string name, Ref<GameplaySystem> system)
	{
		VXM_CORE_ASSERT(!GetInstance().s_Systems.contains(name), "The System '{0}' already exist.", name);
		GetInstance().s_Systems[name] = std::move(system);
		GetInstance().s_SystemToScene[name] = {};
		GetInstance().s_SystemEnabled[name] = true;
		if (Project::ProjectIsLoaded())
		{			
			if (HasSaveFile(name))
			{
				FillSystem(name);
			}
			else
			{
				system->ResetSystem();
			}
		}
		
	}

	Ref<GameplaySystem> SystemManager::GetSystem(const std::string& name)
	{
		VXM_CORE_ASSERT(GetInstance().s_Systems.contains(name), "The System '{0}' doesn't exist.", name);
		return GetInstance().s_Systems[name];
	}

	std::vector<std::string> SystemManager::GetSystemsName()
	{
		std::vector<std::string> names;
		names.reserve(GetInstance().s_Systems.size());
		for (auto &kv: GetInstance().s_Systems) {
			names.push_back(kv.first);
		}
		return names;
	}
	
	bool SystemManager::HasSaveFile(const std::string& name)
	{
		return FileSystem::Exist(GetPath(name));
	}

	void SystemManager::FillSystem(const std::string& name)
	{
		YAML::Node data = FileSystem::ReadFileAsYAML(GetPath(name));
		YAML::Node systemNode = data[name];
		if(!systemNode)
		{
			return;
		}

		Ref<GameplaySystem> system = GetSystem(name);
	
		GetInstance().s_SystemEnabled[name] = systemNode["Enable"].as<bool>();
		auto sceneNodes = systemNode["Scenes"];
		GetInstance().s_SystemToScene[name].clear();
		if(sceneNodes)
		{
			VXM_CORE_ASSERT(sceneNodes.IsSequence(), "The scene node should be a sequence !");
			if(sceneNodes.IsSequence())
			{
				for (auto && sceneNode : sceneNodes)
				{
					GetInstance().s_SystemToScene[name].push_back(sceneNode.as<std::string>());
				}
			}
		}
		system->DeserializeSystem(systemNode);
	} 

	void SystemManager::WriteSystem(YAML::Emitter& out, const std::string& name)
	{
		auto sys = GetSystem(name);
		out << KEYVAL(name, YAML::BeginMap);
		out << KEYVAL("Enable", GetInstance().s_SystemEnabled[name]);
		out << KEYVAL("Scenes", YAML::BeginSeq);
		for (std::string& sceneName : GetInstance().s_SystemToScene[name])
		{
			out << sceneName;
		}
		out << YAML::EndSeq;
		sys->SerializeSystem(out);
		out << YAML::EndMap;
	}

	void SystemManager::SaveSystem(const std::string& name)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		WriteSystem(out, name);
		out << YAML::EndMap;
		FileSystem::WriteYamlFile(GetPath(name), out);
	}
	void SystemManager::LoadSystem(const std::string& name)
	{
		VXM_CORE_ASSERT(GetInstance().s_Systems.contains(name), "The system named {0} doesn't exist...");
		if(HasSaveFile(name)) FillSystem(name);
	}
	
	bool SystemManager::IsActive(const std::string& name)
	{
		VXM_CORE_ASSERT(GetInstance().s_SystemEnabled.contains(name), "The system named {0} doesn't exist...");
		return GetInstance().s_SystemEnabled[name];
	}

	void SystemManager::SetActive(const std::string &systemName, bool enable)
	{
		VXM_CORE_ASSERT(GetInstance().s_SystemEnabled.contains(systemName), "The system named {0} doesn't exist...");
		GetInstance().s_SystemEnabled[systemName] = enable;
	}

	std::vector<std::string>& SystemManager::GetSystemScenes(const std::string& name)
	{
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(name), "The system named {0} doesn't exist...");
		return GetInstance().s_SystemToScene[name];
	}

	
	void SystemManager::AddSceneToSystem(const std::string& systemName, const std::string& sceneName)
	{
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = GetInstance().s_SystemToScene[systemName];
		scenes.push_back(sceneName);
	}

	
	void SystemManager::AddSceneToSystemIfNotExist(const std::string& systemName, const std::string& sceneName)
	{
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = GetInstance().s_SystemToScene[systemName];
		auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
		if(containIndex == scenes.end()) scenes.push_back(sceneName);
	}

	void SystemManager::RemoveSceneFromSystem(const std::string& systemName, const std::string& sceneName)
	{
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = GetInstance().s_SystemToScene[systemName];
		auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
		if(containIndex != scenes.end()) scenes.erase(containIndex);
	}

	std::vector<Ref<GameplaySystem>> SystemManager::GetSystems(const std::string &sceneName)
	{
		std::vector<Ref<GameplaySystem>> systems;
		systems.reserve(GetInstance().s_Systems.size());
		for (auto&& [systemName, system] : GetInstance().s_Systems)
		{
			auto& scenes = GetInstance().s_SystemToScene[systemName];
			auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
			if(containIndex != scenes.end())
			{
				systems.push_back(system);
			}
		}
		return systems;
	}

	SystemManager::SystemManager()
	{
		Project::AddOnLoad(ReloadSystems);
	}

	SystemManager::~SystemManager()
	{
		Project::RemoveOnLoad(ReloadSystems);
	}

	void SystemManager::ReloadSystems()
	{
		for (auto &&[name, sys]: GetInstance().s_Systems)
		{
			if (HasSaveFile(name))
			{
				FillSystem(name);
			}
			else
			{
				sys->ResetSystem();
			}
		}
	}
}

//// ======== CameraControllerSystem ========
//using namespace Voxymore::Core;
//
//VXM_CREATE_SYSTEM(CameraControllerSystem)
//
//void CameraControllerSystem::SerializeSystem(YAML::Emitter &emitter)
//{
//	emitter << KEYVAL("Speed", m_Speed);
//}
//void CameraControllerSystem::DeserializeSystem(YAML::Node &componentNode)
//{
//	m_Speed = componentNode["Speed"].as<float>();
//}
//void CameraControllerSystem::ResetSystem()
//{
//	m_Speed = 5.0f;
//}
//void CameraControllerSystem::OnImGuiRender()
//{
//	ImGui::DragFloat("Speed", &m_Speed);
//};
//void CameraControllerSystem::Update(Scene &scene, TimeStep ts)
//{
//	auto view = scene.GetRegistry().view<TransformComponent, CameraComponent>();
//
//	for (entt::entity entity : view)
//	{
//		if(!view.get<CameraComponent>(entity).Primary) continue;
//		TransformComponent& tc = view.get<TransformComponent>(entity);
//		glm::vec3 localMovement(0);
//		glm::quat rotation = tc.GetRotation();
//		glm::vec3 position = tc.GetPosition();
//
//		if (Input::IsKeyPressed(m_ForwardKey)) localMovement += glm::vec3(0, 0, 1);
//		if (Input::IsKeyPressed(m_BackwardKey)) localMovement += glm::vec3(0, 0, -1);
//		if (Input::IsKeyPressed(m_RightKey)) localMovement += glm::vec3(1, 0, 0);
//		if (Input::IsKeyPressed(m_LeftKey)) localMovement += glm::vec3(-1, 0, 0);
//		if (Input::IsKeyPressed(m_UpKey)) localMovement += glm::vec3(0, 1, 0);
//		if (Input::IsKeyPressed(m_DownKey)) localMovement += glm::vec3(0, -1, 0);
//
//		position += (rotation * localMovement) * (ts * m_Speed);
//		tc.SetPosition(position);
//		return;
//	}
//
//}
