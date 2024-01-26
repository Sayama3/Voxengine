//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include "Voxymore/Core/MultiThreading.hpp"
#include <entt/entt.hpp>
#include <algorithm>
#include <execution>
#include <unordered_set>

// TODO: find a better way?
namespace Voxymore::Editor {
	class SceneHierarchyPanel;
}

namespace Voxymore::Core
{
	class SceneSerializer;
	class Scene;
	class System;

	class Entity
	{
	private:
		entt::entity m_EntityID{entt::null};
		Scene* m_Scene = nullptr;
	public:
		inline Entity() = default;
		inline Entity(const Entity&) = default;
		inline Entity(entt::entity entityID, Scene* scene) : m_EntityID(entityID), m_Scene(scene) {}

		template<typename T>
		inline bool HasComponent() const;

		template<typename T>
		inline T& GetComponent();

		template<typename T>
		inline const T& GetComponent() const;

		template<typename T,  typename... Args>
		inline T& AddComponent(Args &&...args);

		template<typename T>
		inline void AddEmptyComponent();

		template<typename T,  typename... Args>
		inline T& GetOrAddComponent(Args &&...args);

		template<typename T>
		inline void RemoveComponent();



		inline operator bool() const { return IsValid(); }
		inline operator entt::entity() const { return m_EntityID; }
		inline operator uint32_t() const { return static_cast<uint32_t>(m_EntityID); }

		inline bool operator==(const Entity& other) const
		{
			VXM_PROFILE_FUNCTION();
			return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene;
		}

		inline bool operator!=(const Entity& other) const
		{
			VXM_PROFILE_FUNCTION();
			return !(*this == other);
		}


		bool IsValid() const;
		bool IsActive() const;
		void SetActive(bool enable);
		[[deprecated("Use id()")]]
		inline UUID GetUUID() const {return id();}
		UUID id() const;
		UUID scene_id() const;
	};

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
		inline void each(Func& func)
		{
			each<Get...>(MultiThreading::ExecutionPolicy::None, func);
		}

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
		inline void each(entt::exclude_t<Exclude...> ex, Func& func)
		{
			each<Get...>(ex, MultiThreading::ExecutionPolicy::None, func);
		}

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
		inline void each(MultiThreading::ExecutionPolicy exec, Func& func)
		{
			VXM_PROFILE_FUNCTION();
			auto view = m_Registry.view<Get...>();
			MultiThreading::for_each(exec, view.begin(), view.end(), [&view, &func](auto e)
			{
				std::tuple<Get&...> comps = view.template get<Get...>(e);
				auto cs = std::tuple_cat(std::tie(e), comps);
				std::apply(func, cs);
			});
		}

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
		inline void each(entt::exclude_t<Exclude ...> ex, MultiThreading::ExecutionPolicy exec, Func& func)
		{
			VXM_PROFILE_FUNCTION();
			auto view = m_Registry.view<Get ...>(ex);
			MultiThreading::for_each(exec, view.begin(), view.end(), [&view, &func](auto e)
			{
				std::tuple<Get&...> comps = view.template get<Get ...>(e);
				auto cs = std::tuple_cat(std::tie(e), comps);
				std::apply(func, cs);
			});
		}

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
		[[nodiscard]] inline entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<>> view()
		{
			VXM_PROFILE_FUNCTION();
			return m_Registry.view<Get ...>();
		}

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
		[[nodiscard]] inline entt::basic_view<entt::get_t<entt::storage_for_t<Get>...>, entt::exclude_t<entt::storage_for_t<Exclude>...>> view(entt::exclude_t<Exclude...> ex)
		{
			VXM_PROFILE_FUNCTION();
			return m_Registry.view<Get ...>(ex);
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


	struct EntityField
	{
		EntityField(UUID entityId, UUID sceneId);
		explicit EntityField(Entity entity);
		inline EntityField() = default;
		inline ~EntityField() = default;
		UUID EntityId;
		UUID SceneId;

		bool Valid();
		Entity GetEntity(Scene& scene);
	};

	template<typename T>
	inline bool Entity::HasComponent() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Scene->m_Registry.any_of<T>(m_EntityID);
	}

	template<typename T>
	inline T& Entity::GetComponent()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component.", static_cast<uint32_t>(m_EntityID));
		return m_Scene->m_Registry.get<T>(m_EntityID);
	}

	template<typename T>
	inline const T& Entity::GetComponent() const
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component.", static_cast<uint32_t>(m_EntityID));
		return m_Scene->m_Registry.get<T>(m_EntityID);
	}

	template<typename T,  typename... Args>
	inline T& Entity::AddComponent(Args &&...args)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!HasComponent<T>(), "The entity ID: {0} already have the component.", static_cast<uint32_t>(m_EntityID));
		T& component = m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		// Using the operator in case later on we change this to take our Entity
		m_Scene->OnComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T>
	inline void Entity::AddEmptyComponent()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!HasComponent<T>(), "The entity ID: {0} already have the component.", static_cast<uint32_t>(m_EntityID));
		m_Scene->m_Registry.emplace<T>(m_EntityID);
		// Using the operator in case later on we change this to take our Entity
		m_Scene->OnEmptyComponentAdded<T>(*this);
	}

	template<typename T,  typename... Args>
	inline T& Entity::GetOrAddComponent(Args &&...args)
	{
		VXM_PROFILE_FUNCTION();
		if(HasComponent<T>())
		{
			return GetComponent<T>();
		}
		else
		{
			return AddComponent<T>(std::forward<Args>(args)...);
		}
	}

	template<typename T>
	inline void Entity::RemoveComponent()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(HasComponent<T>(), "The entity ID: {0} do not have the component.", static_cast<uint32_t>(m_EntityID));
		m_Scene->m_Registry.remove<T>(m_EntityID);
	}

} // Voxymore
// Core
