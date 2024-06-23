//
// Created by ianpo on 27/01/2024.
//

#pragma once

#include "Scene.forward.hpp"
#include "Entity.decl.hpp"

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/MultiThreading.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include <entt/entt.hpp>
#include <algorithm>
#include <execution>
#include <unordered_set>
#include <unordered_map>

namespace Voxymore::Editor {
	class SceneHierarchyPanel;
}

namespace Voxymore::Core
{
	class SceneSerializer;
	class System;

	class Scene : public Asset
	{
	private:
		friend class ::Voxymore::Editor::SceneHierarchyPanel;
		friend class Entity;
		friend class SceneSerializer;
		friend class System;
	public:
		VXM_IMPLEMENT_ASSET(AssetType::Scene);
	public:
		Scene();
		Scene(std::string name);
		Scene(Ref<Scene> scene);
		Scene(const Scene& scene);
		Scene& operator= (const Scene&);
		~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UUID id, const std::string& name);
		Entity CreateEntity(const std::string& name, UUID id) {return CreateEntity(id,name);}
		Entity CreateEntity(UUID id);
		Entity DuplicateEntity(Entity entity, bool duplicateID = false);

		Entity GetEntity(UUID id);

		void DestroyEntity(Entity entity);

		void StartScene();
		void StopScene();
		inline bool IsStarted() const {return m_Started;}

		void OnUpdateEditor(TimeStep ts, EditorCamera* camera, bool doRendering = true);
		void OnUpdateRuntime(TimeStep ts, bool doRendering = true);

		void RenderEditor(TimeStep ts, EditorCamera& camera);
		void RenderRuntime(TimeStep ts);

		void SetViewportSize(uint32_t width, uint32_t height);

		inline const std::string& GetName() const {return m_Name;}
		inline void SetName(const std::string& name) {m_Name = name;}
		[[deprecated("Use id()")]]
		inline UUID GetID() const {return id();}
		inline UUID id() const {return Handle;}
	public:

		/**
		 * @brief Executes a function on each entity in the registry.
		 *
		 * This template function executes a provided function on each entity in the registry.
		 * The specified components are extracted from the registry using the view method.
		 * The function takes the callable object as a parameter.
		 * The callable object is applied to each entity in the registry.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param func The callable object to apply to each entity.
		 */
		template<typename... Get, typename Func>
		void each(Func& func);

		/**
		 * @brief Executes a function on each entity in the registry that has the specified components.
		 *
		 * This template function executes a provided function on each entity in the registry that
		 * has the specified components. The specified components are extracted from the registry
		 * using the view method. The function takes the callable object as a parameter.
		 * The callable object is applied to each entity that matches the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param func The callable object to apply to each entity.
		 */
		template<typename... Get, typename... Exclude, typename Func>
		void each(entt::exclude_t<Exclude...> ex, Func& func);

		/**
		 * @brief Executes a function on each entity in the registry that has the specified components.
		 *
		 * This template function executes a provided function on each entity in the registry that
		 * has the specified components. The specified components are extracted from the registry
		 * using the view method. The function takes an execution policy and a callable object
		 * as parameters. The execution policy determines how the function is executed.
		 * The callable object is applied to each entity that matches the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param exec The execution policy to control how the function is executed.
		 * @param func The callable object to apply to each entity.
		 */
		template<typename... Get, typename Func>
		void each(MultiThreading::ExecutionPolicy exec, Func& func);

		/**
		 * @brief Executes a function on each entity in the registry that has the specified components.
		 *
		 * This template function executes a provided function on each entity in the registry that
		 * has the specified components. The specified components are extracted from the registry
		 * using the view method. The function takes an execution policy and a callable object
		 * as parameters. The execution policy determines how the function is executed.
		 * The callable object is applied to each entity that matches the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @tparam Exclude Variadic template parameter pack for the types of components to exclude from each entity.
		 * @tparam Func The type of the callable object to apply to each entity.
		 * @param ex The components to exclude from each entity.
		 * @param exec The execution policy to control how the function is executed.
		 * @param func The callable object to apply to each entity.
		 */
		template<typename... Get, typename... Exclude, typename Func>
		void each(entt::exclude_t<Exclude...> ex, MultiThreading::ExecutionPolicy exec, Func& func);

		/**
		 * @brief Creates a view of entities with specified components.
		 *
		 * This template function creates a view of entities in the registry that have the specified components.
		 * The specified components are retrieved from the registry using the view method.
		 * The returned view can be used to iterate over entities with the specified components.
		 *
		 * @tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 * @return entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<>>
		 */
		template<typename... Get>
		[[nodiscard]] entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<>> view();

		/**
		 *@brief Creates a view of entities with specified components.
		 *
		 *This template function creates a view of entities in the registry that have the specified components. The specified components are retrieved from the registry using the view method
		*. The returned view can be used to iterate over entities with the specified components.
		 *
		 *@tparam Get Variadic template parameter pack for the types of components to retrieve from each entity.
		 *@return entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<>>
		 */
		template<typename... Get, typename... Exclude>
		[[nodiscard]] entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<entt::storage_for_t<Exclude>...>> view(entt::exclude_t<Exclude...> ex);

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
}