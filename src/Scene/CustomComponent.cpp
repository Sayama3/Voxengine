//
// Created by ianpo on 21/09/2023.
//

#include "Voxymore/Scene/CustomComponent.hpp"

namespace Voxymore::Core
{
	// ======== CustomComponent ========

	std::vector<ComponentChecker> ComponentManager::s_Components;

	void ComponentManager::AddComponent(const ComponentChecker &component)
	{
		s_Components.push_back(component);
	}

	const std::vector<ComponentChecker> &ComponentManager::GetComponents()
	{
		return s_Components;
	}

	bool ComponentManager::HasComponent(const std::string &componentName)
	{
		for (auto& c : s_Components)
		{
			if(c.ComponentName == componentName) {
				return true;
			}
		}
		return false;
	}

}
// ======== BoatComponent ========
using namespace Voxymore::Core;

VXM_CREATE_COMPONENT(BoatComponent);

bool BoatComponent::HasComponent(Entity e)
{
	return e.HasComponent<BoatComponent>();
}
void BoatComponent::DeserializeComponent(YAML::Node &componentNode, Entity targetEntity)
{
	auto &boat = targetEntity.GetOrAddComponent<BoatComponent>();
	boat.m_Speed = componentNode["Speed"].as<float>();
}
void BoatComponent::SerializeComponent(YAML::Emitter &emitter, Entity sourceEntity)
{
	auto& boat = sourceEntity.GetComponent<BoatComponent>();
	emitter << KEYVAL("Speed", boat.m_Speed);
}

void BoatComponent::OnImGuiRender(Entity sourceEntity)
{
	auto& boat = sourceEntity.GetComponent<BoatComponent>();
	ImGui::DragFloat("Speed", &boat.m_Speed);
}

std::string BoatComponent::GetName()
{
	return "BoatComponent";
}
