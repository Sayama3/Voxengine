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
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto camerasGroup = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto entity : camerasGroup) {
			auto&& [transform, camera] = camerasGroup.get<TransformComponent, CameraComponent>(entity);

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

			auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			for (auto entity: group) {
				auto &&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
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

		auto cameraGroup = m_Registry.group<CameraComponent>();
		for (auto entity : cameraGroup) {
			auto& camera = cameraGroup.get<CameraComponent>(entity);
			if(!camera.FixedAspectRation)
			{
				camera.Camera.SetViewportSize(width, height);
			}
		}
	}
} // Voxymore
// Core