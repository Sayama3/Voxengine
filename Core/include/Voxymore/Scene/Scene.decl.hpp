//
// Created by ianpo on 27/01/2024.
//

#pragma once

#include "Scene.forward.hpp"
#include "Entity.decl.hpp"

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/MultiThreading.hpp"

#include "Voxymore/Assets/Asset.hpp"

#include "Voxymore/Renderer/EditorCamera.hpp"

#include "Voxymore/Physics/ObjectLayerPairFilter.hpp"
#include "Voxymore/Physics/BroadPhaseLayerInterface.hpp"
#include "Voxymore/Physics/PhysicsListener.hpp"
#include "Voxymore/Physics/ObjectVsBroadPhaseLayerFilter.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <entt/entt.hpp>

namespace Voxymore::Editor {
	class SceneHierarchyPanel;
}

namespace Voxymore::Core
{
	class SceneSerializer;
	class System;
	class RigidbodyComponent;
	struct Light;

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
		void RenderLoop();

		void SetViewportSize(uint32_t width, uint32_t height);

		inline const std::string& GetName() const {return m_Name;}
		inline void SetName(const std::string& name) {m_Name = name;}
		[[deprecated("Use id()")]]
		inline UUID GetID() const {return id();}
		inline UUID id() const {return Handle;}
	private:
		std::vector<Light> FindAllLights();
	private:
		void StartPhysics();
		void UpdatePhysics(TimeStep ts);
		void StopPhysics();
	private:
		void UpdatePhysicsState();
		void CreatePhysicsBody(entt::entity e, RigidbodyComponent& rb);
		void UpdatePhysicsBody(entt::entity e, RigidbodyComponent& rb);
		void UpdateShape(entt::entity e, RigidbodyComponent& rb);
	private:
		static const JPH::Shape* GetShape(Entity entity);
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
	private:
		JPH::PhysicsSystem m_PhysicsSystem;

		/// Create mapping table from object layer to broadphase layer
		/// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		BroadPhaseLayerInterface m_BroadPhaseLayer;

		/// Create class that filters object vs broadphase layers
		/// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		ObjectVsBroadPhaseLayerFilter m_ObjectVsBroadphaseLayerFilter;

		/// Create class that filters object vs object layers
		/// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		ObjectLayerPairFilter m_ObjectVsObjectLayerFilter;

		/// A body activation listener gets notified when bodies activate and go to sleep
		/// Note that this is called from a job so whatever you do here needs to be thread safe.
		/// Registering one is entirely optional.
		LoggerActivationListener m_BodyActivationListener;

		/// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
		/// Note that this is called from a job so whatever you do here needs to be thread safe.
		/// Registering one is entirely optional.
		LoggerContactListener m_ContactListener;

		JPH::BodyInterface* m_BodyInterface = nullptr;
	public:
		[[deprecated("Use the 'each<>()' function to get the desire result.")]]
		inline entt::registry& GetRegistry() { return m_Registry; }
		[[deprecated("Use the 'each<>()' function to get the desire result.")]]
		inline const entt::registry& GetRegistry() const { return m_Registry; }
	};
}