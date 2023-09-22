//
// Created by ianpo on 21/09/2023.
//

#include <utility>

#include "Voxymore/Scene/GameplaySystem.hpp"
#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core
{
	std::unordered_map<std::string, Ref<GameplaySystem>> SystemManager::s_Systems;
	std::unordered_map<std::string, std::vector<std::string>> SystemManager::s_SystemToScene;
	std::unordered_map<std::string, bool> SystemManager::s_SystemEnabled;

	Path SystemManager::GetPath(const std::string& name)
	{
		return {FileSource::System, name};
	}
	void SystemManager::AddSystem(std::string name, Ref<GameplaySystem> system)
	{
		VXM_CORE_ASSERT(!s_Systems.contains(name), "The System '{0}' already exist.", name);
		s_Systems[name] = std::move(system);
		s_SystemToScene[name] = {};
		s_SystemEnabled[name] = true;
		if(HasSaveFile(name))
		{
			FillSystem(name);
		}
	}

	Ref<GameplaySystem> SystemManager::GetSystem(const std::string& name)
	{
		VXM_CORE_ASSERT(s_Systems.contains(name), "The System '{0}' doesn't exist.", name);
		return s_Systems[name];
	}

	std::vector<std::string> SystemManager::GetSystemsName()
	{
		std::vector<std::string> names;
		names.reserve(s_Systems.size());
		for (auto &kv: s_Systems) {
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
	
		s_SystemEnabled[name] = systemNode["Enable"].as<bool>();
		auto sceneNodes = systemNode["Scenes"];
		s_SystemToScene[name].clear();
		if(sceneNodes)
		{
			VXM_CORE_ASSERT(sceneNodes.IsSequence(), "The scene node should be a sequence !");
			if(sceneNodes.IsSequence())
			{
				for (auto && sceneNode : sceneNodes)
				{
					s_SystemToScene[name].push_back(sceneNode.as<std::string>());
				}
			}
		}
		system->DeserializeSystem(systemNode);
	} 

	void SystemManager::WriteSystem(YAML::Emitter& out, const std::string& name)
	{
		auto sys = GetSystem(name);
		out << KEYVAL(name, YAML::BeginMap);
		out << KEYVAL("Enable", s_SystemEnabled[name]);
		out << KEYVAL("Scenes", YAML::BeginSeq);
		for (std::string& sceneName : s_SystemToScene[name])
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
		if(HasSaveFile(name)) FillSystem(name);
	}
	
	bool SystemManager::SystemIsEnable(const std::string& name)
	{
		VXM_CORE_ASSERT(s_SystemEnabled.contains(name), "The system named {0} doesn't exist...");
		return s_SystemEnabled[name];
	}

	std::vector<std::string>& SystemManager::GetSystemScenes(const std::string& name)
	{
		VXM_CORE_ASSERT(s_SystemToScene.contains(name), "The system named {0} doesn't exist...");
		return s_SystemToScene[name];
	}

	
	void SystemManager::AddSceneToSystem(const std::string& systemName, const std::string& sceneName)
	{
		VXM_CORE_ASSERT(s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = s_SystemToScene[systemName];
		scenes.push_back(sceneName);
	}

	
	void SystemManager::AddSceneToSystemIfNotExist(const std::string& systemName, const std::string& sceneName)
	{
		VXM_CORE_ASSERT(s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = s_SystemToScene[systemName];
		auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
		if(containIndex == scenes.end()) scenes.push_back(sceneName);
	}

	void SystemManager::RemoveSceneFromSystem(const std::string& systemName, const std::string& sceneName)
	{
		VXM_CORE_ASSERT(s_SystemToScene.contains(systemName), "The system named {0} doesn't exist...");
		auto& scenes = s_SystemToScene[systemName];
		auto containIndex = std::find(scenes.begin(), scenes.end(), sceneName);
		if(containIndex != scenes.end()) scenes.erase(containIndex);
	}
}

// ======== CameraControllerSystem ========
using namespace Voxymore::Core;

VXM_CREATE_SYSTEM(CameraControllerSystem)

void CameraControllerSystem::SerializeSystem(YAML::Emitter &emitter)
{
	emitter << KEYVAL("Speed", m_Speed);
}
void CameraControllerSystem::DeserializeSystem(YAML::Node &componentNode)
{
	m_Speed = componentNode["Speed"].as<float>();
}
void CameraControllerSystem::OnImGuiRender()
{
	ImGui::DragFloat("Speed", &m_Speed);
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