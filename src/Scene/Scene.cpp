//
// Created by ianpo on 24/08/2023.
//

#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/Components.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Renderer/Renderer.hpp"


namespace Voxymore::Core
{
	static uint32_t count = 0;
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		// TODO: make it happen only when the scene play !
		{
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
				Renderer::Submit(mesh.Material, mesh.Mesh, transform.GetTransform());
			}

			Renderer::EndScene();
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity{m_Registry.create(), this};

		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "SceneEntity_" +std::to_string(count++) : name;

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
} // Voxymore
// Core