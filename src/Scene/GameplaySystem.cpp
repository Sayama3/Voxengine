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