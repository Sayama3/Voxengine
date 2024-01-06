//
// Created by ianpo on 21/09/2023.
//

#include <utility>

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Scene/Systems.hpp"

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
		VXM_PROFILE_FUNCTION();
		return {FileSource::System, name + VXM_SYSTEM_EXTENSION};
	}
	void SystemManager::AddSystem(std::string name, Ref<System> system)
	{
		VXM_PROFILE_FUNCTION();
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

	Ref<System> SystemManager::GetSystem(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_Systems.contains(name), "The System '{0}' doesn't exist.", name);
		return GetInstance().s_Systems[name];
	}

	std::vector<std::string> SystemManager::GetSystemsName()
	{
		VXM_PROFILE_FUNCTION();
		std::vector<std::string> names;
		names.reserve(GetInstance().s_Systems.size());
		for (auto &kv: GetInstance().s_Systems) {
			names.push_back(kv.first);
		}
		return names;
	}
	
	bool SystemManager::HasSaveFile(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		return FileSystem::Exist(GetPath(name));
	}

	void SystemManager::ResetSystem(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		GetInstance().s_SystemEnabled[name] = true;
		GetInstance().s_SystemToScene[name].clear();
		Ref<System> system = GetSystem(name);
		system->ResetSystem();
	}

	void SystemManager::FillSystem(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		YAML::Node data = FileSystem::ReadFileAsYAML(GetPath(name));
		YAML::Node systemNode = data[name];
		if(!systemNode)
		{
			return;
		}

		Ref<System> system = GetSystem(name);

		GetInstance().s_SystemEnabled[name] = systemNode["Enable"].as<bool>();
		auto sceneNodes = systemNode["Scenes"];
		GetInstance().s_SystemToScene[name].clear();
		if(sceneNodes)
		{
			if(sceneNodes.IsSequence())
			{
				for (auto && sceneNode : sceneNodes)
				{
					GetInstance().s_SystemToScene[name].emplace_back(sceneNode.as<uint64_t>());
				}
			}
		}
		system->DeserializeSystem(systemNode);
	}

	void SystemManager::WriteSystem(YAML::Emitter& out, const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		auto sys = GetSystem(name);
		out << KEYVAL(name, YAML::BeginMap);
		out << KEYVAL("Enable", GetInstance().s_SystemEnabled[name]);
		out << KEYVAL("Scenes", YAML::BeginSeq);
		for (UUID& sceneId : GetInstance().s_SystemToScene[name])
		{
			out << sceneId;
		}
		out << YAML::EndSeq;
		sys->SerializeSystem(out);
		out << YAML::EndMap;
	}

	void SystemManager::SaveSystem(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		YAML::Emitter out;
		out << YAML::BeginMap;
		WriteSystem(out, name);
		out << YAML::EndMap;
		FileSystem::WriteYamlFile(GetPath(name), out);
	}

	void SystemManager::LoadSystem(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_Systems.contains(name), "The system named {0} doesn't exist...");
		if(HasSaveFile(name)) FillSystem(name);
	}

	bool SystemManager::IsActive(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_SystemEnabled.contains(name), "The system named {0} doesn't exist...");
		return GetInstance().s_SystemEnabled[name];
	}

	void SystemManager::SetActive(const std::string &systemName, bool enable)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_SystemEnabled.contains(systemName), "The system named {0} doesn't exist...");
		GetInstance().s_SystemEnabled[systemName] = enable;
	}

	std::vector<UUID>& SystemManager::GetSystemScenes(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(name), "The system named {0} doesn't exist...");
		return GetInstance().s_SystemToScene[name];
	}


	void SystemManager::AddSceneToSystem(const std::string& systemName, UUID sceneName)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = GetInstance().s_SystemToScene[systemName];
		scenes.push_back(sceneName);
	}


	void SystemManager::AddSceneToSystemIfNotExist(const std::string& systemName, UUID sceneName)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = GetInstance().s_SystemToScene[systemName];
		auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
		if(containIndex == scenes.end()) scenes.push_back(sceneName);
	}

	void SystemManager::RemoveSceneFromSystem(const std::string& systemName, UUID sceneName)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(GetInstance().s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = GetInstance().s_SystemToScene[systemName];
		auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
		if(containIndex != scenes.end()) scenes.erase(containIndex);
	}

	std::vector<Ref<System>> SystemManager::GetSystems(UUID sceneName)
	{
		VXM_PROFILE_FUNCTION();
		std::vector<Ref<System>> systems;
		systems.reserve(GetInstance().s_Systems.size());
		for (auto&& [systemName, system] : GetInstance().s_Systems)
		{
			if(system->RunOnAllScenes())
			{
				systems.push_back(system);
			}
			else
			{
				auto& scenes = GetInstance().s_SystemToScene[systemName];
				auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
				if (containIndex != scenes.end()) {
					systems.push_back(system);
				}
			}
		}
		return systems;
	}

	SystemManager::SystemManager()
	{
		VXM_PROFILE_FUNCTION();
		m_OnProjectLoadId = Project::AddOnLoad(ReloadSystems);
	}

	SystemManager::~SystemManager()
	{
		VXM_PROFILE_FUNCTION();
		Project::RemoveOnLoad(m_OnProjectLoadId);
	}

	void SystemManager::ReloadSystems()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_INFO("Reload Systems");
		for (auto &&[name, sys]: GetInstance().s_Systems)
		{
			if (HasSaveFile(name))
			{
				VXM_CORE_INFO("Reload System {0}.", name);
				FillSystem(name);
			}
			else
			{
				VXM_CORE_INFO("Reset System {0}.", name);
				ResetSystem(name);
			}
		}
	}
}
/*
// ======== CameraControllerSystem ========
using namespace Voxymore::Core;

VXM_DECLARE_SYSTEM(CameraControllerSystem)

void CameraControllerSystem::SerializeSystem(YAML::Emitter &emitter)
{
	System::SerializeSystem(emitter);
	emitter << KEYVAL("Speed", m_Speed);
}
void CameraControllerSystem::DeserializeSystem(YAML::Node &componentNode)
{
	System::DeserializeSystem(componentNode);
	m_Speed = componentNode["Speed"].as<float>();
}
void CameraControllerSystem::ResetSystem()
{
	m_Speed = 5.0f;
}
bool CameraControllerSystem::OnImGuiRender()
{
	return ImGui::DragFloat("Speed", &m_Speed);
};
void CameraControllerSystem::Update(Scene &scene, TimeStep ts)
{
	auto view = scene.GetRegistry().view<TransformComponent, CameraComponent>();

	for (entt::entity entity : view)
	{
		if(!view.get<CameraComponent>(entity).Primary) continue;
		TransformComponent& tc = view.get<TransformComponent>(entity);
		glm::vec3 localMovement(0);
		glm::quat rotation = tc.GetRotation();
		glm::vec3 position = tc.GetPosition();

		if (Input::IsKeyPressed(m_ForwardKey)) localMovement += glm::vec3(0, 0, 1);
		if (Input::IsKeyPressed(m_BackwardKey)) localMovement += glm::vec3(0, 0, -1);
		if (Input::IsKeyPressed(m_RightKey)) localMovement += glm::vec3(1, 0, 0);
		if (Input::IsKeyPressed(m_LeftKey)) localMovement += glm::vec3(-1, 0, 0);
		if (Input::IsKeyPressed(m_UpKey)) localMovement += glm::vec3(0, 1, 0);
		if (Input::IsKeyPressed(m_DownKey)) localMovement += glm::vec3(0, -1, 0);

		position += (rotation * localMovement) * (ts * m_Speed);
		tc.SetPosition(position);
		return;
	}

}
*/
