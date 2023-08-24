//
// Created by ianpo on 24/08/2023.
//

#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/Components.hpp"
#include "Voxymore/Renderer/Renderer.hpp"


namespace Voxymore::Core
{
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

	entt::entity Scene::CreateEntity()
	{
		entt::entity entity = m_Registry.create();
		return entity;
	}
	entt::registry& Scene::Reg()
	{
		return m_Registry;
	}
} // Voxymore
// Core