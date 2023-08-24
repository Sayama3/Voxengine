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
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto&& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
			Renderer::Submit(mesh.Mat, mesh.Mesh, transform.GetTransform());
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = {m_Registry.create(), this};

		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? &"SceneEntity_" [ count++] : name;

		return entity;
	}
} // Voxymore
// Core