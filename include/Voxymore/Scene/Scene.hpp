//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include <entt/entt.hpp>

// TODO: find a better way?
namespace Voxymore::Editor {
	class SceneHierarchyPanel;
}

//TODO: add a way to create/delete system (that can go onto the whole scene) instead of juste NativeScriptComponent.
namespace Voxymore::Core
{
	class SceneSerializer;
	class Entity;
	struct TagComponent;
	struct TransformComponent;
	struct MeshComponent;
	struct CameraComponent;
	struct NativeScriptComponent;
	class GameplaySystem;

	class Scene
	{
	private:
		friend class Voxymore::Editor::SceneHierarchyPanel;
		friend class Entity;
		friend class SceneSerializer;
		friend class GameplaySystem;
	public:
		Scene();
		Scene(std::string name);
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void OnUpdateRuntime(TimeStep ts);
		void SetViewportSize(uint32_t width, uint32_t height);

		void AddSystem(Ref<GameplaySystem>& system);
		void RemoveSystem(Ref<GameplaySystem>& system);

		inline const std::string& GetName() const {return m_Name;}
		inline void SetName(const std::string& name) {m_Name = name;}
	private:
		template<typename T>
		void OnComponentAdded(entt::entity entity, T& component);
	public:
		// Helper:
		Entity GetPrimaryCameraEntity();
	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		entt::registry m_Registry;
		std::vector<Ref<GameplaySystem>> m_Systems;
		std::string m_Name;
	private:
		inline entt::registry& GetRegistry() { return m_Registry; }
		inline const entt::registry& GetRegistry() const { return m_Registry; }
	};
	template<>
	void Scene::OnComponentAdded<TagComponent>(entt::entity entity, TagComponent& tagComponent);

	template<>
	void Scene::OnComponentAdded<TransformComponent>(entt::entity entity, TransformComponent& transformComponent);

	template<>
	void Scene::OnComponentAdded<MeshComponent>(entt::entity entity, MeshComponent& meshComponent);

	template<>
	void Scene::OnComponentAdded<CameraComponent>(entt::entity entity, CameraComponent& cameraComponent);

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(entt::entity entity, NativeScriptComponent& nativeScriptComponent);

} // Voxymore
// Core
