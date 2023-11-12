//
// Created by ianpo on 24/08/2023.
//

#include <utility>

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Components/ModelComponent.hpp"
#include "Voxymore/Debug/Instrumentor.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/GameplaySystem.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"


namespace Voxymore::Core
{
	Scene::Scene() : m_ID(), m_Name("Scene_"+std::to_string(m_ID))
	{
	}

	Scene::Scene(std::string name) : m_ID(), m_Name(std::move(name))
	{
	}

	Scene::Scene(UUID id) : m_ID(id), m_Name("Scene_"+std::to_string(m_ID))
	{

	}

	Scene::Scene(UUID id, std::string name) : m_ID(id), m_Name(std::move(name))
	{

	}

	Scene::Scene(const Scene& scene) : m_ID(), m_Name(scene.m_Name), m_ViewportHeight(scene.m_ViewportHeight), m_ViewportWidth(scene.m_ViewportWidth)
	{
		Path cacheScene = {FileSource::Cache, "./Scenes/"+std::to_string(scene.m_ID)+".vxm"};
		std::string cacheSceneStr = cacheScene.GetFullPath().string();
		// Casting to Scene* for because I know I won't edit the scene on the serialize function but still need it as raw non-const pointer.
		SceneSerializer cacheSerializer((Scene*)&scene);
		cacheSerializer.Serialize(cacheSceneStr);
		// Change target to deserialize the data to the current scene
		cacheSerializer.ChangeSceneTarget(this);
		cacheSerializer.Deserialize(cacheSceneStr, false);
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		VXM_PROFILE_FUNCTION();

		{
			auto models = m_Registry.view<ModelComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : models)
			{
				auto& model = models.get<ModelComponent>(entity);
				if(model.ShouldLoad()) model.LoadModel();
			}
		}

		Renderer::BeginScene(camera);
		{
			auto modelsView = m_Registry.view<ModelComponent, TransformComponent>(entt::exclude<DisableComponent>);
			for (auto entity: modelsView) {
				auto&& [transform, model] = modelsView.get<TransformComponent, ModelComponent>(entity);
				if(model.IsLoaded())
				{
					VXM_CORE_INFO("Submit Model {0}", model.GetLocalPath().string());
					Renderer::Submit(model.GetModel(), transform.GetTransform(), static_cast<int>(entity));
				}
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();

		{
			auto models = m_Registry.view<ModelComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : models)
			{
				auto& model = models.get<ModelComponent>(entity);
				if(model.ShouldLoad()) model.LoadModel();
			}
		}

		// TODO: make it happen only when the scene play !
		{
			VXM_PROFILE_SCOPE("Scene::OnUpdateRuntime -> Update systems");
			auto systems = SystemManager::GetSystems(GetName());
			for (Ref<GameplaySystem>& system : systems)
			{
				if(SystemManager::IsActive(system->GetName())) system->Update(*this, ts);
			}
		}

		{
			VXM_PROFILE_SCOPE("Scene::OnUpdateRuntime -> Update NativeScriptComponent");
			m_Registry.view<NativeScriptComponent>(entt::exclude<DisableComponent>).each([=, this](auto entity, NativeScriptComponent& nsc)
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

		auto camerasView = m_Registry.view<CameraComponent, TransformComponent>(entt::exclude<DisableComponent>);
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

			{
				auto modelsView = m_Registry.view<ModelComponent, TransformComponent>(entt::exclude<DisableComponent>);
				for (auto entity: modelsView) {
					auto&& [transform, model] = modelsView.get<TransformComponent, ModelComponent>(entity);
					if(model.IsLoaded()) Renderer::Submit(model.GetModel(), transform.GetTransform(), static_cast<int>(entity));
				}
			}

			Renderer::EndScene();
		}
	}

	Entity Scene::CreateEntity()
	{
		UUID id;
		std::string entity = "Entity_"+std::to_string(id);
		return CreateEntity(id, entity);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(UUID id)
	{
		std::string entity = "Entity_"+std::to_string(id);
		return CreateEntity(id, entity);
	}

	Entity Scene::CreateEntity(UUID id, const std::string& name)
	{
		Entity entity = Entity{m_Registry.create(), this};

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name;

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

	//    template<typename T>
//    void Scene::OnComponentAdded(entt::entity entity, T& tagComponent)
//    {
//    }
} // Voxymore
// Core