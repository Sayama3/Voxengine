//
// Created by ianpo on 21/09/2023.
//

#include "Voxymore/Scene/GameplaySystem.hpp"

namespace Voxymore::Core
{
	std::unordered_map<std::string, Ref<GameplaySystem>> SystemManager::s_Systems;
	void SystemManager::AddSystem(std::string name, Ref<GameplaySystem> system)
	{
		VXM_CORE_ASSERT(!s_Systems.contains(name), "The System '{0}' already exist.", name);
		s_Systems[name] = system;
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
void CameraControllerSystem::Update(Scene &scene, TimeStep ts)
{

};