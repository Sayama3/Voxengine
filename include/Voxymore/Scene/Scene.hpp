//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include "Voxymore/Core/Core.hpp"
#include <entt/entt.hpp>

// TODO: find a better way?
namespace Voxymore::Editor {
	class SceneHierarchyPanel;
}

//TODO: remove all the redifinitions of class by using the unified component system.
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
		Scene(UUID id);
		Scene(UUID id, std::string name);
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(UUID id, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void OnUpdateRuntime(TimeStep ts);
		void SetViewportSize(uint32_t width, uint32_t height);

		inline const std::string& GetName() const {return m_Name;}
		inline void SetName(const std::string& name) {m_Name = name;}
		inline UUID GetID() const {return m_ID;}
	private:
		template<typename T>
		inline void OnComponentAdded(entt::entity entity, T& component) {}
	public:
		// Helper:
		Entity GetPrimaryCameraEntity();
	private:
		UUID m_ID;
		std::string m_Name;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		entt::registry m_Registry;
	public:
		inline entt::registry& GetRegistry() { return m_Registry; }
		inline const entt::registry& GetRegistry() const { return m_Registry; }
	};

} // Voxymore
// Core
