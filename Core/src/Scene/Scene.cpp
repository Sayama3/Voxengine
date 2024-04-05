//
// Created by ianpo on 24/08/2023.
//

#include <utility>

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/ModelComponent.hpp"
#include "Voxymore/Components/PrimitiveComponent.hpp"
#include "Voxymore/Components/BezierCurve.hpp"
#include "Voxymore/Components/GenericBezierCurve.hpp"
#include "Voxymore/Debug/Profiling.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"
#include "Voxymore/Scene/Systems.hpp"
#include "Voxymore/Components/LightComponent.hpp"


namespace Voxymore::Core
{
	Scene::Scene() : m_ID(), m_Name("Scene_"+std::to_string(m_ID))
	{
		VXM_PROFILE_FUNCTION();
		InitScene();
	}

	Scene::Scene(std::string name) : m_ID(), m_Name(std::move(name))
	{
		VXM_PROFILE_FUNCTION();
		InitScene();
	}

	Scene::Scene(UUID id) : m_ID(id), m_Name("Scene_"+std::to_string(m_ID))
	{
		VXM_PROFILE_FUNCTION();
		InitScene();
	}

	Scene::Scene(UUID id, std::string name) : m_ID(id), m_Name(std::move(name))
	{
		VXM_PROFILE_FUNCTION();
		InitScene();
	}

	Scene::Scene(Ref<Scene> scene) : m_ID(scene->m_ID), m_Name(scene->m_Name), m_ViewportHeight(scene->m_ViewportHeight), m_ViewportWidth(scene->m_ViewportWidth)
	{
		VXM_PROFILE_FUNCTION();
		InitScene();
		// Copy the scene here, we want to retrieve each entity upon creation on this scene.
		Path cacheScene = {FileSource::Cache, "./Scenes/"+std::to_string(scene->m_ID)+".vxm"};
		// Casting to Scene* for because I know I won't edit the scene on the serialize function but still need it as a raw non-const pointer.
		SceneSerializer cacheSerializer(scene);
		cacheSerializer.Serialize(cacheScene);
		// Change target to deserialize the data to the current scene
		cacheSerializer.ChangeSceneTarget(this);
		cacheSerializer.Deserialize(cacheScene);
	}

	Scene::Scene(const Scene &scene) : m_ID(scene.m_ID), m_Name(scene.m_Name), m_ViewportHeight(scene.m_ViewportHeight), m_ViewportWidth(scene.m_ViewportWidth)
	{
		VXM_PROFILE_FUNCTION();
		InitScene();

		// Copy the scene here, we want to retrieve each entity upon creation on this scene.
		Path cacheScene = {FileSource::Cache, "./Scenes/"+std::to_string(scene.m_ID)+".vxm"};
		// Casting to Scene* for because I know I won't edit the scene on the serialize function but still need it as a raw non-const pointer.
		SceneSerializer cacheSerializer((Scene*)&scene);
		cacheSerializer.Serialize(cacheScene);
		// Change target to deserialize the data to the current scene
		cacheSerializer.ChangeSceneTarget(this);
		cacheSerializer.Deserialize(cacheScene);
	}


	Scene &Scene::operator=(const Scene & scene)
	{
		VXM_PROFILE_FUNCTION();
		m_ID = scene.m_ID;
		m_Name = scene.m_Name;
		m_ViewportHeight = scene.m_ViewportHeight;
		m_ViewportWidth = scene.m_ViewportWidth;

		// Copy the scene here, we want to retrieve each entity upon creation on this scene.
		Path cacheScene = {FileSource::Cache, "./Scenes/"+std::to_string(scene.m_ID)+".vxm"};
		// Casting to Scene* for because I know I won't edit the scene on the serialize function but still need it as a raw non-const pointer.
		SceneSerializer cacheSerializer((Scene*)&scene);
		cacheSerializer.Serialize(cacheScene);
		// Change target to deserialize the data to the current scene
		cacheSerializer.ChangeSceneTarget(this);
		cacheSerializer.Deserialize(cacheScene);

		return *this;
	}

	void Scene::InitScene()
	{
		VXM_PROFILE_FUNCTION();
		m_Registry.on_construct<IDComponent>().connect<&Scene::OnCreateIDComponent>(this);
		m_Registry.on_destroy<IDComponent>().connect<&Scene::OnDestroyIDComponent>(this);
	}

	Scene::~Scene()
	{
		VXM_PROFILE_FUNCTION();
		m_Registry.on_construct<IDComponent>().disconnect<&Scene::OnCreateIDComponent>(this);
		m_Registry.on_destroy<IDComponent>().disconnect<&Scene::OnDestroyIDComponent>(this);
	}

	void Scene::OnCreateIDComponent(entt::entity e)
	{
		VXM_PROFILE_FUNCTION();
		Entity entity(e, this);
		m_Entities[entity.GetUUID()] = entity;
	}

	void Scene::OnDestroyIDComponent(entt::entity e)
	{
		VXM_PROFILE_FUNCTION();
		Entity entity(e, this);
		auto it = m_Entities.find(entity.id());
		if(it != m_Entities.end())
		{
			m_Entities.erase(it);
		}
		else
		{
			VXM_CORE_WARNING("The entity ({0}) was not found in the entity map.", entity.GetUUID());
		}
	}

	void Scene::StartScene()
	{
		VXM_PROFILE_FUNCTION();
		auto systems = SystemManager::GetSystems(m_ID);
		std::unordered_set<std::string> toStopSystems = m_StartedSystem;
		for (Ref<System>& system : systems)
		{
			std::string systemName = system->GetName();
			if(SystemManager::IsActive(systemName))
			{
				auto it = toStopSystems.find(systemName);
				if(it != toStopSystems.end())toStopSystems.erase(it);
				if(!m_StartedSystem.contains(systemName))
				{
					system->OnStart(*this);
					m_StartedSystem.insert(systemName);
				}
			}
		}

		for(auto& toStopSystem : toStopSystems)
		{
			SystemManager::GetSystem(toStopSystem)->OnStop(*this);
			m_StartedSystem.erase(toStopSystem);
		}
		m_Started = true;
	}

	void Scene::StopScene()
	{
		VXM_PROFILE_FUNCTION();
		std::unordered_set<std::string> toStopSystems = m_StartedSystem;
		for(auto& system : toStopSystems)
		{
			SystemManager::GetSystem(system)->OnStop(*this);
			m_StartedSystem.erase(system);
		}
		m_Started = false;
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

		{
			auto primitives = m_Registry.view<PrimitiveComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : primitives)
			{
				auto& pc = primitives.get<PrimitiveComponent>(entity);
				if(pc.IsDirty()) pc.Load();
			}
		}

		std::vector<Light> lights;
		{
			auto lightsComps = m_Registry.view<LightComponent, TransformComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : lightsComps)
			{
				auto&& [lc, tc] = lightsComps.get<LightComponent, TransformComponent>(entity);
				lights.push_back(lc.AsLight(tc));
			}
		}

		Renderer::BeginScene(camera, lights);
		{
			auto bezierView = m_Registry.view<BezierCurve, TransformComponent>(entt::exclude<DisableComponent>);
			for (auto entity: bezierView) {
				auto&& [bezier, transform] = bezierView.get<BezierCurve, TransformComponent>(entity);
				Mat4 trs = transform.GetTransform();
				std::vector<Vertex> controlPoints = {Vertex(), Vertex(), Vertex(), Vertex()};
				for (int i = 0; i < 4; ++i) {
					controlPoints[i].Position = Math::TransformPoint(trs, bezier.LocalControlPoints[i]);
				}
				Renderer::Submit(controlPoints, bezier.Definition, bezier.GetShaderName(), static_cast<int>(entity));
			}

			auto genericBezierView = m_Registry.view<GenericBezierCurve, TransformComponent>(entt::exclude<DisableComponent>);
			for(auto entity : genericBezierView)
			{
				auto&& [bezier, transform] = genericBezierView.get<GenericBezierCurve, TransformComponent>(entity);

				auto points = bezier.GetWorldPoints(transform.GetTransform());

				std::vector<Vertex> controlPoints(bezier.GetTotalControlPoints());
				const auto count = (points.size()/bezier.GetTotalControlPoints()) * bezier.GetTotalControlPoints();
				for (int i = 0; i < count; i+= bezier.GetTotalControlPoints()) {
					for (int j = 0; j < bezier.GetTotalControlPoints(); ++j) {
						controlPoints[j].Position = points[i+j];
					}
					Renderer::Submit(controlPoints, bezier.Definition, bezier.GetShaderName(), static_cast<int>(entity));
				}
			}

			auto modelsView = m_Registry.view<ModelComponent, TransformComponent>(entt::exclude<DisableComponent>);
			for (auto entity: modelsView) {
				auto&& [transform, model] = modelsView.get<TransformComponent, ModelComponent>(entity);
				if(model.IsLoaded())
				{
					Renderer::Submit(model.GetModel(), transform.GetTransform(), static_cast<int>(entity));
				}
			}

			auto primitives = m_Registry.view<PrimitiveComponent, TransformComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : primitives)
			{
				auto&& [pc, transform] = primitives.get<PrimitiveComponent, TransformComponent>(entity);
				if(pc.IsLoaded())
				{
					Renderer::Submit(pc.GetMesh(), transform.GetTransform(), static_cast<int>(entity));
				}
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();


		// TODO: See if I really should reload on Runtime the models...
		{
			auto models = m_Registry.view<ModelComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : models)
			{
				auto& model = models.get<ModelComponent>(entity);
				if(model.ShouldLoad()) {
					model.LoadModel();
				}
			}
		}

		// TODO: See if I really should reload on Runtime the primitives...
		{
			auto primitives = m_Registry.view<PrimitiveComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : primitives)
			{
				auto& pc = primitives.get<PrimitiveComponent>(entity);
				if(pc.IsDirty()) {
					pc.Load();
				}
			}
		}

		VXM_CORE_CHECK(m_Started, "The scene '{0}' is updating but hasn't been started.", m_Name);

		{
			VXM_PROFILE_SCOPE("Scene::OnUpdateRuntime -> Update systems");
			auto systems = SystemManager::GetSystems(m_ID);
			std::unordered_set<std::string> toStopSystems = m_StartedSystem;
			for (Ref<System>& system : systems)
			{
				std::string systemName = system->GetName();
				if(SystemManager::IsActive(systemName))
				{
					auto it = toStopSystems.find(systemName);
					if(it != toStopSystems.end()) toStopSystems.erase(it);
					if(!m_StartedSystem.contains(systemName))
					{
						system->OnStart(*this);
						m_StartedSystem.insert(systemName);
					}
					else
					{
						system->Update(*this, ts);
					}
				}
			}

			for(auto& toStopSystem : toStopSystems)
			{
				SystemManager::GetSystem(toStopSystem)->OnStop(*this);
				m_StartedSystem.erase(toStopSystem);
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
			std::vector<Light> lights;
			{
				auto lightsComps = m_Registry.view<LightComponent, TransformComponent>(entt::exclude<DisableComponent>);
				for (entt::entity entity : lightsComps)
				{
					auto&& [lc, tc] = lightsComps.get<LightComponent, TransformComponent>(entity);
					lights.push_back(lc.AsLight(tc));
				}
			}

			Renderer::BeginScene(*mainCamera, cameraTransform, lights);


			auto bezierView = m_Registry.view<BezierCurve, TransformComponent>(entt::exclude<DisableComponent>);
			for (auto entity: bezierView) {
				auto&& [bezier, transform] = bezierView.get<BezierCurve, TransformComponent>(entity);
				Mat4 trs = transform.GetTransform();
				std::vector<Vertex> controlPoints = {Vertex(), Vertex(), Vertex(), Vertex()};
				for (int i = 0; i < 4; ++i) {
					controlPoints[i].Position = Math::TransformPoint(trs, bezier.LocalControlPoints[i]);
				}
				Renderer::Submit(controlPoints, bezier.Definition, bezier.GetShaderName(), static_cast<int>(entity));
			}

			auto genericBezierView = m_Registry.view<GenericBezierCurve, TransformComponent>(entt::exclude<DisableComponent>);
			for(auto entity : genericBezierView)
			{
				auto&& [bezier, transform] = genericBezierView.get<GenericBezierCurve, TransformComponent>(entity);

				auto points = bezier.GetWorldPoints(transform.GetTransform());

				std::vector<Vertex> controlPoints(bezier.GetTotalControlPoints());
				const auto count = (points.size()/bezier.GetTotalControlPoints()) * bezier.GetTotalControlPoints();
				for (int i = 0; i < count; i+= bezier.GetTotalControlPoints()) {
					for (int j = 0; j < bezier.GetTotalControlPoints(); ++j) {
						controlPoints[j].Position = points[i+j];
					}
					Renderer::Submit(controlPoints, bezier.Definition, bezier.GetShaderName(), static_cast<int>(entity));
				}
			}

			{
				auto modelsView = m_Registry.view<ModelComponent, TransformComponent>(entt::exclude<DisableComponent>);
				for (auto entity: modelsView) {
					auto&& [transform, model] = modelsView.get<TransformComponent, ModelComponent>(entity);
					if(model.IsLoaded())
					{
						Renderer::Submit(model.GetModel(), transform.GetTransform(), static_cast<int>(entity));
					}
				}
			}

			auto primitives = m_Registry.view<PrimitiveComponent, TransformComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : primitives)
			{
				auto&& [pc, transform] = primitives.get<PrimitiveComponent, TransformComponent>(entity);
				if(pc.IsLoaded())
				{
					Renderer::Submit(pc.GetMesh(), transform.GetTransform(), static_cast<int>(entity));
				}
			}

			Renderer::EndScene();
		}
	}

	Entity Scene::CreateEntity()
	{
		VXM_PROFILE_FUNCTION();
		UUID id;
		std::string entity = "Entity_"+std::to_string(id);
		return CreateEntity(id, entity);
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		std::string entity = "Entity_"+std::to_string(id);
		return CreateEntity(id, entity);
	}

	Entity Scene::CreateEntity(UUID id, const std::string& name)
	{
		VXM_PROFILE_FUNCTION();
		Entity entity = Entity{m_Registry.create(), this};

		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name;

		return entity;
	}

	Entity Scene::GetEntity(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		auto it = m_Entities.find(id);
		if(it != m_Entities.end()) {
			return it->second;
		}
		else {
			VXM_CORE_ERROR("Entity ID({0}) not found.", id);
			return {};
		}
	}

	void Scene::SetViewportSize(uint32_t width, uint32_t height)
	{
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(entity.IsValid(), "Scene can only destroy valid entity.");
		m_Registry.destroy(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		VXM_PROFILE_FUNCTION();
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
} // Voxymore
// Core