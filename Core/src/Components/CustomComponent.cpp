//
// Created by ianpo on 21/09/2023.
//

#include "Voxymore/Components/CustomComponent.hpp"

namespace Voxymore::Core
{
	// ======== CustomComponent ========

//	std::vector<ComponentChecker> ComponentManager::s_Components;
	ComponentManager* ComponentManager::s_ComponentManager = nullptr;
	ComponentManager& ComponentManager::GetInstance()
	{
		VXM_PROFILE_FUNCTION();
		if(s_ComponentManager == nullptr) s_ComponentManager = new ComponentManager();
		return *s_ComponentManager;
	}

	void ComponentManager::AddComponent(const ComponentChecker &component)
	{
		VXM_PROFILE_FUNCTION();
		GetInstance().s_Components.push_back(component);
	}

	const std::vector<ComponentChecker> &ComponentManager::GetComponents()
	{
		VXM_PROFILE_FUNCTION();
		return GetInstance().s_Components;
	}

	bool ComponentManager::HasComponent(const std::string &componentName)
	{
		VXM_PROFILE_FUNCTION();
		for (auto& c : GetInstance().s_Components)
		{
			if(c.ComponentName == componentName) {
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool ComponentCreator<T>::s_Created = false;
}
//// ======== BoatComponent ========
//using namespace Voxymore::Core;
//
//VXM_CREATE_COMPONENT(BoatComponent);
//
//void BoatComponent::DeserializeComponent(YAML::Node &componentNode, Entity targetEntity)
//{
//	auto &boat = targetEntity.GetOrAddComponent<BoatComponent>();
//	boat.m_Speed = componentNode["Speed"].as<float>();
//}
//
//void BoatComponent::SerializeComponent(YAML::Emitter &emitter, Entity sourceEntity)
//{
//	auto& boat = sourceEntity.GetComponent<BoatComponent>();
//	emitter << KEYVAL("Speed", boat.m_Speed);
//}
//
//bool BoatComponent::OnImGuiRender(Entity sourceEntity)
//{
//	auto& boat = sourceEntity.GetComponent<BoatComponent>();
//	return ImGui::DragFloat("Speed", &boat.m_Speed);
//}
