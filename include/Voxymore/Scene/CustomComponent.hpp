//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include <string>
#include <vector>
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/Components.hpp"
#include "Voxymore/Core/YamlHelper.hpp"

// ======== CustomComponent ========
namespace Voxymore::Core
{
	struct ComponentChecker
	{
		std::string ComponentName;
		bool (*HasComponent)(Entity);
		void (*SerializeComponent)(YAML::Emitter &/*Emitter*/, Entity /*sourceEntity*/); //TODO: change for my own "serializer"
		void (*DeserializeComponent)(YAML::Node& /*componentNode*/, Entity/*targetEntity*/); // TODO: Change for own "deserializer"
	};

	class ComponentManager
	{
	private:
		static std::vector<ComponentChecker> s_Components;
	public:
		static void AddComponent(const ComponentChecker& component);
		static bool HasComponent(const std::string&componentName);
		static const std::vector<ComponentChecker>& GetComponents();
	};

	template<typename T>
	class ComponentCreator
	{
	private:
		static bool s_Created;
	public:
		inline ComponentCreator()
		{
			std::string name = T::GetName();
			if(ComponentManager::HasComponent(name)) return;
			ComponentChecker cc;
			cc.ComponentName = name;
			cc.HasComponent = T::HasComponent;
			cc.SerializeComponent = T::SerializeComponent;
			cc.DeserializeComponent = T::DeserializeComponent;
			ComponentManager::AddComponent(cc);
		}
	};

}
#define VXM_IMPLEMENT_COMPONENT() public:\
	static bool HasComponent(::Voxymore::Core::Entity e);\
	static void DeserializeComponent(YAML::Node& componentNode, ::Voxymore::Core::Entity targetEntity); \
	static void SerializeComponent(YAML::Emitter& Emitter, ::Voxymore::Core::Entity sourceEntity); \
	static std::string GetName();

#define VXM_CREATE_COMPONENT(COMP) ::Voxymore::Core::ComponentCreator<COMP> VXM_COMBINE(s_ComponentCreator, __LINE__)

// ======== BoatComponent ========
class BoatComponent
{
public:
	VXM_IMPLEMENT_COMPONENT();
private:
	float m_Speed = 10.0f;
};