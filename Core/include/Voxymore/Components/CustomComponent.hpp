//
// Created by ianpo on 21/09/2023.
//

#pragma once

#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include <imgui.h>
#include <string>
#include <vector>
#include <static_block.hpp>

// ======== CustomComponent ========
namespace Voxymore::Core
{
	struct ComponentChecker
	{
public:
		std::string ComponentName;
		uint64_t ComponentHash;

		bool (*HasComponent)(Entity);
		void (*SerializeComponent)(YAML::Emitter &/*emitter*/, Entity /*sourceEntity*/); //TODO: change for my own "serializer"
		void (*DeserializeComponent)(YAML::Node& /*componentNode*/, Entity/*targetEntity*/); // TODO: Change for own "deserializer"
		bool (*OnImGuiRender)(Entity/*sourceEntity*/);
		bool (*OnImGuizmo)(Entity/*sourceEntity*/, const float* viewMatrix, const float* projectionMatrix) = nullptr;
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
		static void RemoveComponent(const std::string&componentName);
		static const std::vector<ComponentChecker>& GetComponents();
	};
	template<class...>struct types{using type=types;};

	template<class Sig> struct args;
	template<class R, class...Args>
	struct args<R(Args...)>:types<Args...>{};
	template<class Sig> using args_t=typename args<Sig>::type;

	template <class...Params>
	void some_function(types<Params...>) {

	}

	template<class T>
	class Component
	{
	public:
		inline static bool HasComponent(::Voxymore::Core::Entity e) {return e.HasComponent<T>();}
		inline static void AddComponent(::Voxymore::Core::Entity e) { e.AddComponent<T>(); }
		inline static void RemoveComponent(::Voxymore::Core::Entity e) { e.RemoveComponent<T>(); }
		inline static void StaticDeserializeComponent(YAML::Node& node, ::Voxymore::Core::Entity targetEntity) {targetEntity.GetComponent<T>().DeserializeComponent(node);}
		inline static void StaticSerializeComponent(YAML::Emitter& out, ::Voxymore::Core::Entity sourceEntity) {sourceEntity.GetComponent<T>().SerializeComponent(out);}
		inline static bool StaticOnImGuiRender(::Voxymore::Core::Entity sourceEntity) {return sourceEntity.GetComponent<T>().OnImGuiRender();}
		inline static bool StaticOnImGuizmo(::Voxymore::Core::Entity sourceEntity, const float* viewMatrix, const float* projectionMatrix) {return sourceEntity.GetComponent<T>().OnImGuizmo(viewMatrix, projectionMatrix);}
		inline static std::string StaticGetName() { return T::GetName(); }

		inline static void RegisterComponent()
		{
			std::string name = StaticGetName();
			if(ComponentManager::HasComponent(name)) return;
			ComponentChecker cc;
			cc.ComponentName = name;
			cc.ComponentHash = typeid(T).hash_code();
			cc.HasComponent = T::HasComponent;
			cc.AddComponent = T::AddComponent;
			cc.RemoveComponent = T::RemoveComponent;
			cc.SerializeComponent = T::StaticSerializeComponent;
			cc.DeserializeComponent = T::StaticDeserializeComponent;
			cc.OnImGuiRender = T::StaticOnImGuiRender;
			cc.OnImGuizmo = T::StaticOnImGuizmo;
			ComponentManager::AddComponent(cc);
		}

		inline static void UnregisterComponent()
		{
			std::string name = StaticGetName();
			ComponentManager::RemoveComponent(name);
		}
	};


	template<class T>
	class SelfAwareComponent
	{
	public:
		inline static bool HasComponent(::Voxymore::Core::Entity e) {return e.HasComponent<T>();}
		inline static void AddComponent(::Voxymore::Core::Entity e) { e.AddComponent<T>(); }
		inline static void RemoveComponent(::Voxymore::Core::Entity e) { e.RemoveComponent<T>(); }
		inline static void StaticDeserializeComponent(YAML::Node& node, ::Voxymore::Core::Entity targetEntity) {targetEntity.GetComponent<T>().DeserializeComponent(node, targetEntity);}
		inline static void StaticSerializeComponent(YAML::Emitter& out, ::Voxymore::Core::Entity sourceEntity) {sourceEntity.GetComponent<T>().SerializeComponent(out, sourceEntity);}
		inline static bool StaticOnImGuiRender(::Voxymore::Core::Entity sourceEntity) {return sourceEntity.GetComponent<T>().OnImGuiRender(sourceEntity);}
		inline static bool StaticOnImGuizmo(::Voxymore::Core::Entity sourceEntity, const float* viewMatrix, const float* projectionMatrix) {return sourceEntity.GetComponent<T>().OnImGuizmo(sourceEntity, viewMatrix, projectionMatrix);}
		inline static std::string StaticGetName() { return T::GetName(); }

		inline static void RegisterComponent()
		{
			std::string name = StaticGetName();
			if(ComponentManager::HasComponent(name)) return;
			ComponentChecker cc;
			cc.ComponentName = name;
			cc.ComponentHash = typeid(T).hash_code();
			cc.HasComponent = T::HasComponent;
			cc.AddComponent = T::AddComponent;
			cc.RemoveComponent = T::RemoveComponent;
			cc.SerializeComponent = T::StaticSerializeComponent;
			cc.DeserializeComponent = T::StaticDeserializeComponent;
			cc.OnImGuiRender = T::StaticOnImGuiRender;
			cc.OnImGuizmo = T::StaticOnImGuizmo;
			ComponentManager::AddComponent(cc);
		}

		inline static void UnregisterComponent()
		{
			std::string name = StaticGetName();
			ComponentManager::RemoveComponent(name);
		}
	};
}

#define VXM_IMPLEMENT_COMPONENT(COMP) public: inline static std::string GetName() { return #COMP;} inline bool OnImGuizmo(const float* viewMatrix, const float* projectionMatrix) {return false;}
#define VXM_IMPLEMENT_SELFAWARECOMPONENT(COMP) public: inline static std::string GetName() { return #COMP;} inline bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix) {return false;}
#define VXM_IMPLEMENT_NAME(COMP) public: inline static std::string GetName() { return #COMP;}


#define VXM_CREATE_COMPONENT(COMP) static_block{COMP::RegisterComponent();}

#define VXM_CREATE_LIGHT_COMPONENT(COMP) VXM_CREATE_COMPONENT(COMP)

//// ======== BoatComponent ========
//class BoatComponent
//{
//	VXM_IMPLEMENT_COMPONENT(BoatComponent);
//private:
//	float m_Speed = 10.0f;
//};