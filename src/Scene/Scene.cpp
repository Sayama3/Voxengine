//
// Created by ianpo on 24/08/2023.
//

#include <utility>

#include "Voxymore/Debug/Instrumentor.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/Components.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/GameplaySystem.hpp"
#include "Voxymore/Renderer/Renderer.hpp"


namespace Voxymore::Core
{
	static uint32_t entityCount = 0;
	static uint32_t sceneCount = 0;
	Scene::Scene() : m_Name("NoName")
	{
	  m_Name.append(std::to_string(sceneCount++));
	}
	Scene::Scene(std::string name) : m_Name(std::move(name))
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		VXM_PROFILE_FUNCTION();
		Renderer::BeginScene(camera);

		auto meshesView = m_Registry.view<MeshComponent, TransformComponent>();
		for (entt::entity entity : meshesView) {
			auto [transform, mesh] = meshesView.get<TransformComponent, MeshComponent>(entity);
			Renderer::Submit(mesh.Material, mesh.Mesh, transform.GetTransform(), static_cast<int>(entity));
		}

		Renderer::EndScene();
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();

		// TODO: make it happen only when the scene play !
		{
			VXM_PROFILE_SCOPE("Scene::OnUpdateRuntime -> Update systems");
			for (Ref<GameplaySystem>& system : m_Systems)
			{
				system->Update(*this, ts);
			}
		}

		{
			VXM_PROFILE_SCOPE("Scene::OnUpdateRuntime -> Update NativeScriptComponent");
			m_Registry.view<NativeScriptComponent>().each([=, this](auto entity, NativeScriptComponent& nsc)
		  	{
			 	if(!nsc.IsValid())
				{
					nsc.CreateInstance();
					nsc.Instance->m_Entity = Entity{entity, this};
					nsc.Instance->OnCreate();
			 	}
				nsc.Instance->OnUpdate(ts);
		  	});
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto camerasView = m_Registry.view<CameraComponent, TransformComponent>();
		for (auto entity : camerasView) {
			auto [transform, camera] = camerasView.get<TransformComponent, CameraComponent>(entity);

			if(camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.GetTransform();
				break;
			}
		}

		if(mainCamera)
		{
			Renderer::BeginScene(*mainCamera, cameraTransform);

			auto meshesView = m_Registry.view<MeshComponent, TransformComponent>();
			for (auto entity: meshesView) {
				auto [transform, mesh] = meshesView.get<TransformComponent, MeshComponent>(entity);
				Renderer::Submit(mesh.Material, mesh.Mesh, transform.GetTransform(), static_cast<int>(entity));
			}

			Renderer::EndScene();
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity{m_Registry.create(), this};

		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "SceneEntity_" +std::to_string(entityCount++) : name;

		return entity;
	}

	void Scene::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto entity : cameraView) {
			auto& camera = cameraView.get<CameraComponent>(entity);
			if(!camera.FixedAspectRatio)
			{
				camera.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::DestroyEntity(Entity entity)
	{
		VXM_CORE_ASSERT(entity.IsValid(), "Scene can only destroy valid entity.");
		m_Registry.destroy(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto cameraView = m_Registry.view<CameraComponent>();
		for (auto entity : cameraView)
		{
			auto& camera = cameraView.get<CameraComponent>(entity);
			if(camera.Primary)
			{
				return Entity(entity, this);
			}
		}
		return Entity();
	}

	void Scene::AddSystem(Ref<GameplaySystem>& system)
	{
		system->OnAttachToScene(*this);
		m_Systems.push_back(system);
	}

	void Scene::RemoveSystem(Ref<GameplaySystem>& system)
	{
		int systemIndex = FindSystem(system);
		VXM_CORE_ASSERT(systemIndex >= 0, "The system isn't part of the Scene.");
		system->OnDetachFromScene(*this);
		m_Systems.erase(m_Systems.begin() + systemIndex);
	}

	int Scene::FindSystem(Ref<GameplaySystem> &system) const
	{
		for (int i = 0; i < m_Systems.size(); ++i)
		{
			if(m_Systems[i]->GetName() == system->GetName())
			{
				return i;
			}
		}
		return -1;
	}

	template<typename T>
	void Scene::OnComponentAdded(entt::entity entity, T& tagComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(entt::entity entity, TagComponent& tagComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(entt::entity entity, TransformComponent& transformComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<MeshComponent>(entt::entity entity, MeshComponent& meshComponent)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(entt::entity entity, CameraComponent& cameraComponent)
	{
		if(!cameraComponent.FixedAspectRatio)
		{
			cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(entt::entity entity, NativeScriptComponent& nativeScriptComponent)
	{
	}
} // Voxymore
// Core