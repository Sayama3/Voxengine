//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include <entt/entt.hpp>
#include <unordered_set>

// TODO: find a better way?
namespace Voxymore::Editor {
	class SceneHierarchyPanel;
}

namespace Voxymore::Core
{
	class SceneSerializer;
	class Entity;
	class System;

	template<typename... Type>
	inline constexpr entt::exclude_t<Type...> exclude{};

	class Scene
	{
	private:
		friend class Voxymore::Editor::SceneHierarchyPanel;
		friend class Entity;
		friend class SceneSerializer;
		friend class System;
	public:
		Scene();
		Scene(std::string name);
		Scene(UUID id);
		Scene(UUID id, std::string name);
		Scene(Ref<Scene> scene);
		Scene(const Scene& scene);
		Scene& operator= (const Scene&);
		~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UUID id, const std::string& name);
		Entity CreateEntity(UUID id);

		Entity GetEntity(UUID id);

		void DestroyEntity(Entity entity);

		void StartScene();
		void StopScene();
		inline bool IsStarted() const {return m_Started;}

		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void OnUpdateRuntime(TimeStep ts);
		void SetViewportSize(uint32_t width, uint32_t height);

		inline const std::string& GetName() const {return m_Name;}
		inline void SetName(const std::string& name) {m_Name = name;}
		[[deprecated("Use id()")]]
		inline UUID GetID() const {return id();}
		inline UUID id() const {return m_ID;}
	public:
		template<typename... Get, typename Func>
		void each(Func func)
		{
			auto view = m_Registry.view<Get ...>();
			view.each(func);
		}

		template<typename... Get, typename... Exclude, typename Func>
		void each(entt::exclude_t<Exclude...> ex, Func func)
		{
			auto view = m_Registry.view<Get ...>(ex);
			view.each(func);
		}
	private:
		template<typename T>
		inline void OnComponentAdded(entt::entity entity, T& component) {}
		template<typename T>
		inline void OnEmptyComponentAdded(entt::entity entity) {}
		void InitScene();
		void OnCreateIDComponent(entt::entity);
		void OnDestroyIDComponent(entt::entity);
	public:
		// Helper:
		Entity GetPrimaryCameraEntity();
	private:
		UUID m_ID;
		std::string m_Name;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		entt::registry m_Registry;
		std::unordered_set<std::string> m_StartedSystem;
		std::unordered_map<UUID, Entity> m_Entities;
		bool m_Started = false;
	public:
		[[deprecated("Use the 'each<>()' function to get the desire result.")]]
		inline entt::registry& GetRegistry() { return m_Registry; }
		[[deprecated("Use the 'each<>()' function to get the desire result.")]]
		inline const entt::registry& GetRegistry() const { return m_Registry; }
	};

} // Voxymore
// Core
