//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include <string>
#include <vector>
#include <imgui.h>
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Core/YamlHelper.hpp"

// ======== CustomComponent ========
namespace Voxymore::Core
{
	struct ComponentChecker
	{
public:
		std::string ComponentName;
		uint64_t ComponentHash;

		bool (*HasComponent)(Entity);
		void (*SerializeComponent)(YAML::Emitter &/*Emitter*/, Entity /*sourceEntity*/); //TODO: change for my own "serializer"
		void (*DeserializeComponent)(YAML::Node& /*componentNode*/, Entity/*targetEntity*/); // TODO: Change for own "deserializer"
		void (*OnImGuiRender)(Entity/*sourceEntity*/);
		void (*AddComponent)(Entity);
		void (*RemoveComponent)(Entity);
	};

	class ComponentManager
	{
	private:
		std::vector<ComponentChecker> s_Components;
		static ComponentManager* s_ComponentManager;
		static ComponentManager& GetInstance();
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
			VXM_CORE_ASSERT(!s_Created, "The Component {0} has already been created.", typeid(T).name())
			std::string name = T::GetName();
			if(ComponentManager::HasComponent(name)) return;
			ComponentChecker cc;
			cc.ComponentName = name;
			cc.ComponentHash = typeid(T).hash_code();
			cc.HasComponent = T::HasComponent;
			cc.AddComponent = T::AddComponent;
			cc.RemoveComponent = T::RemoveComponent;
			cc.SerializeComponent = T::SerializeComponent;
			cc.DeserializeComponent = T::DeserializeComponent;
			cc.OnImGuiRender = T::OnImGuiRender;
			ComponentManager::AddComponent(cc);
			s_Created = true;
		}
	};

}
#define VXM_IMPLEMENT_COMPONENT(COMP) private:\
	static ::Voxymore::Core::ComponentCreator<COMP> s_ComponentCreator;\
public:\
	inline static bool HasComponent(::Voxymore::Core::Entity e) {return e.HasComponent<COMP>();}\
	inline static void AddComponent(::Voxymore::Core::Entity e) { e.AddComponent<COMP>(); }\
	inline static void RemoveComponent(::Voxymore::Core::Entity e) { e.RemoveComponent<COMP>(); }\
	static void DeserializeComponent(YAML::Node& componentNode, ::Voxymore::Core::Entity targetEntity); \
	static void SerializeComponent(YAML::Emitter& Emitter, ::Voxymore::Core::Entity sourceEntity); \
	static void OnImGuiRender(::Voxymore::Core::Entity sourceEntity); \
	static void DrawComponent(::Voxymore::Core::Entity sourceEntity); \
	inline static std::string GetName() { return #COMP; }

#define VXM_CREATE_COMPONENT(COMP) ::Voxymore::Core::ComponentCreator<COMP> COMP::s_ComponentCreator; \
template<> \
void ::Voxymore::Core::Scene::OnComponentAdded<COMP>(entt::entity entity, COMP& component){} \
template<> \
bool ::Voxymore::Core::ComponentCreator<COMP>::s_Created = false;

#define VXM_CREATE_LIGHT_COMPONENT(COMP) ::Voxymore::Core::ComponentCreator<COMP> COMP::s_ComponentCreator; \
template<> \
bool ::Voxymore::Core::ComponentCreator<COMP>::s_Created = false;

//// ======== BoatComponent ========
//class BoatComponent
//{
//	VXM_IMPLEMENT_COMPONENT(BoatComponent);
//private:
//	float m_Speed = 10.0f;
//};