//
// Created by ianpo on 24/08/2023.
//

#include <utility>

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/ModelComponent.hpp"
#include "Voxymore/Components/PrimitiveComponent.hpp"
#include "Voxymore/Debug/Profiling.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"
#include "Voxymore/Scene/Systems.hpp"
#include "Voxymore/Components/LightComponent.hpp"
#include "Voxymore/Core/Application.hpp"


#include "Voxymore/Physics/ObjectLayerPairFilter.hpp"
#include "Voxymore/Physics/BroadPhaseLayerInterface.hpp"
#include "Voxymore/Physics/PhysicsListener.hpp"
#include "Voxymore/Physics/ObjectVsBroadPhaseLayerFilter.hpp"
#include "Voxymore/Physics/PhysicsLayer.hpp"
#include "Voxymore/Physics/RigidbodyComponent.hpp"
#include "Voxymore/Physics/ColliderComponent.hpp"
#include "Voxymore/Physics/PhysicsDebugRenderer.hpp"
#include "Voxymore/Physics/PhysicsTypeHelpers.hpp"


#include "Voxymore/Scene/Scene.decl.hpp"


#include <Jolt/Jolt.h>
// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>


namespace Voxymore::Core
{
	Scene::Scene() : m_Name("Scene_"+std::to_string(Handle))
	{
		VXM_PROFILE_FUNCTION();
		InitScene();
	}

	Scene::Scene(std::string name) : m_Name(std::move(name))
	{
		VXM_PROFILE_FUNCTION();
		InitScene();
	}

	Scene::Scene(Ref<Scene> scene) : m_Name(scene->m_Name), m_ViewportHeight(scene->m_ViewportHeight), m_ViewportWidth(scene->m_ViewportWidth)
	{
		VXM_PROFILE_FUNCTION();
		Handle = scene->Handle;
		InitScene();
		// Copy the scene here, we want to retrieve each entity upon creation on this scene.
		Path cacheScene = {FileSource::Cache, "./Scenes/"+std::to_string(scene->Handle)+".vxm"};
		// Casting to Scene* for because I know I won't edit the scene on the serialize function but still need it as a raw non-const pointer.
		SceneSerializer cacheSerializer(scene);
		cacheSerializer.Serialize(cacheScene);
		// Change target to deserialize the data to the current scene
		cacheSerializer.ChangeSceneTarget(this);
		cacheSerializer.Deserialize(cacheScene);
	}

	Scene::Scene(const Scene &scene) : m_Name(scene.m_Name), m_ViewportHeight(scene.m_ViewportHeight), m_ViewportWidth(scene.m_ViewportWidth)
	{
		VXM_PROFILE_FUNCTION();
		Handle = scene.Handle;
		InitScene();

		// Copy the scene here, we want to retrieve each entity upon creation on this scene.
		Path cacheScene = {FileSource::Cache, "./Scenes/"+std::to_string(scene.Handle)+".vxm"};
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
		Handle = scene.Handle;
		m_Name = scene.m_Name;
		m_ViewportHeight = scene.m_ViewportHeight;
		m_ViewportWidth = scene.m_ViewportWidth;

		// Copy the scene here, we want to retrieve each entity upon creation on this scene.
		Path cacheScene = {FileSource::Cache, "./Scenes/"+std::to_string(scene.Handle)+".vxm"};
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


		m_PhysicsSystem.Init(PhysicsLayer::cMaxBodies, PhysicsLayer::cNumBodyMutexes, PhysicsLayer::cMaxBodyPairs, PhysicsLayer::cMaxContactConstraints, m_BroadPhaseLayer, m_ObjectVsBroadphaseLayerFilter, m_ObjectVsObjectLayerFilter);

		// A body activation listener gets notified when bodies activate and go to sleep
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.
		m_PhysicsSystem.SetBodyActivationListener(&m_BodyActivationListener);

		// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.
		m_PhysicsSystem.SetContactListener(&m_ContactListener);

		// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
		// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
		m_BodyInterface = &m_PhysicsSystem.GetBodyInterface();
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
			VXM_CORE_WARNING("The entity ({0}) was not found in the entity map.", entity.id().string());
		}
	}

	void Scene::StartScene()
	{
		VXM_PROFILE_FUNCTION();
		auto systems = SystemManager::GetSystems(Handle);
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
		StartPhysics();
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
		StopPhysics();
		m_Started = false;
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera* camera, bool doRendering)
	{
		VXM_PROFILE_FUNCTION();

		{
			auto primitives = m_Registry.view<PrimitiveComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : primitives)
			{
				auto& pc = primitives.get<PrimitiveComponent>(entity);
				if(pc.IsDirty()) pc.Load();
			}
		}

		UpdatePhysicsState();

		if(doRendering) {
			VXM_ASSERT(camera, "Camera cannot be null if we do the Editor Rendering.");
			RenderEditor(ts, *camera);
		}
	}

	void Scene::OnUpdateRuntime(TimeStep ts, bool doRendering)
	{
		VXM_PROFILE_FUNCTION();

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
			auto systems = SystemManager::GetSystems(Handle);
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

		UpdatePhysicsState();
		UpdatePhysics(ts);

		if(doRendering) {
			RenderRuntime(ts);
		}
	}

	void Scene::RenderEditor(TimeStep ts, EditorCamera& camera)
	{
		VXM_PROFILE_FUNCTION();
		std::vector<Light> lights;
		{
			auto lightsComps = m_Registry.view<LightComponent, TransformComponent>(entt::exclude<DisableComponent>);
			for (entt::entity entity : lightsComps)
			{
				auto&& [lc, tc] = lightsComps.get<LightComponent, TransformComponent>(entity);
				lights.push_back(lc.AsLight(tc));
			}
		}
		Entity mainCam = GetPrimaryCameraEntity();
		CameraComponent* cc = mainCam && mainCam.HasComponent<CameraComponent>() ? &mainCam.GetComponent<CameraComponent>() : nullptr;

		CubemapField cubemap = cc ? cc->Cubemap : NullAssetHandle;
		ShaderField cubemapShader = cc ? cc->CubemapShader : NullAssetHandle;

		Renderer::BeginScene(camera, lights, cubemap, cubemapShader);

		RenderLoop();

		Renderer::EndScene();
	}

	void Scene::RenderRuntime(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		CubemapField cubemap = NullAssetHandle;
		ShaderField cubemapShader = NullAssetHandle;

		auto camerasView = m_Registry.view<CameraComponent, TransformComponent>(entt::exclude<DisableComponent>);
		for (auto entity : camerasView) {
			auto [transform, camera] = camerasView.get<TransformComponent, CameraComponent>(entity);

			if(camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.GetTransform();
				cubemap = camera.Cubemap;
				cubemapShader = camera.CubemapShader;
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

			Renderer::BeginScene(*mainCamera, cameraTransform, lights, cubemap, cubemapShader);

			RenderLoop();

			Renderer::EndScene();
		}
	}

	void Scene::RenderLoop()
	{
		auto modelsView = m_Registry.view<ModelComponent, TransformComponent>(entt::exclude<DisableComponent>);
		for (auto entity: modelsView) {
			auto&& [transform, model] = modelsView.get<TransformComponent, ModelComponent>(entity);
			if(model.IsValid())
			{
				Renderer::Submit(model.GetModel(), transform.GetTransform(), static_cast<int>(entity));
			}
		}

		auto primitives = m_Registry.view<PrimitiveComponent, TransformComponent>(entt::exclude<DisableComponent>);
		for (entt::entity entity : primitives)
		{
			auto&& [pc, transform] = primitives.get<PrimitiveComponent, TransformComponent>(entity);
			if(pc.IsValid())
			{
				Renderer::Submit(pc.GetMesh(), transform.GetTransform(), static_cast<int>(entity));
			}
		}

		// Draw Physics
		if(JPH::DebugRenderer::sInstance == nullptr) {
			JPH::DebugRenderer::sInstance = new PhysicsDebugRenderer();
		}
		Entity camera = GetPrimaryCameraEntity();
		if(camera) {
			auto pos = camera.GetComponent<TransformComponent>().GetPosition();
			reinterpret_cast<PhysicsDebugRenderer*>(JPH::DebugRenderer::sInstance)->SetCameraPos({pos.x, pos.y, pos.z});
		}

		// TODO: Fetch the 'JPH::BodyManager::DrawSettings' from some project settings somewhere
		static JPH::BodyManager::DrawSettings s_DrawSettings {
				false, 												// mDrawGetSupportFunction
				false, 												// mDrawSupportDirection
				false, 												// mDrawGetSupportingFace
				true, 												// mDrawShape
				true, 												// mDrawShapeWireframe
				JPH::BodyManager::EShapeColor::SleepColor,			// mDrawShapeColor
				false, 												// mDrawBoundingBox
				true, 												// mDrawCenterOfMassTransform
				false, 												// mDrawWorldTransform
				true, 												// mDrawVelocity
				false, 												// mDrawMassAndInertia
				false, 												// mDrawSleepStats
				false, 												// mDrawSoftBodyVertices
				false, 												// mDrawSoftBodyVertexVelocities
				false, 												// mDrawSoftBodyEdgeConstraints
				false, 												// mDrawSoftBodyBendConstraints
				false, 												// mDrawSoftBodyVolumeConstraints
				false, 												// mDrawSoftBodySkinConstraints
				false, 												// mDrawSoftBodyLRAConstraints
				false, 												// mDrawSoftBodyPredictedBounds
		};

		m_PhysicsSystem.DrawBodies(s_DrawSettings, JPH::DebugRenderer::sInstance);
		m_PhysicsSystem.DrawConstraints(JPH::DebugRenderer::sInstance);
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

	Entity Scene::DuplicateEntity(Entity entity, bool duplicateID)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(entity, "The Entity is invalid.");

		if(!entity)
		{
			return {};
		}

		Entity duplicateEntity;

		if(duplicateID) {
			duplicateEntity = CreateEntity(entity.GetComponent<TagComponent>().Tag, entity.id());
		} else {
			duplicateEntity = CreateEntity(entity.GetComponent<TagComponent>().Tag);
		}

		duplicateEntity.SetActive(entity.IsActive());

		if (entity.HasComponent<TransformComponent>()) {
			auto& dtc = duplicateEntity.GetOrAddComponent<TransformComponent>();
			dtc = entity.GetComponent<TransformComponent>();
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& dc = duplicateEntity.GetOrAddComponent<CameraComponent>();
			dc = entity.GetComponent<CameraComponent>();
		}

		for (const ComponentChecker &cc: ComponentManager::GetComponents()) {
			if (cc.HasComponent(entity)) {
				cc.DuplicateComponent(entity, duplicateEntity);
			}
		}

		return duplicateEntity;
	}

	Entity Scene::GetEntity(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		auto it = m_Entities.find(id);
		if(it != m_Entities.end()) {
			return it->second;
		}
		else {
			VXM_CORE_ERROR("Entity ID({0}) not found.", id.string());
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

	void Scene::StartPhysics()
	{
		if(!CheckHasPhysicsLayer()) return;

		VXM_CORE_ASSERT(m_BodyInterface, "The body interface is not initialized.");
		if(!m_BodyInterface) return;

		UpdatePhysicsState();
		each<RigidbodyComponent>([this](entt::entity, RigidbodyComponent& rc) {
			if(rc.m_BodyID.IsInvalid()) return;
			JPH::BodyLockWrite lock{m_PhysicsSystem.GetBodyLockInterfaceNoLock(), rc.m_BodyID};
			if(lock.Succeeded()) {
				rc.m_Body = &lock.GetBody();
			}
		});
		m_PhysicsSystem.OptimizeBroadPhase();
	}

	void Scene::UpdatePhysics(TimeStep ts)
	{
		if(!CheckHasPhysicsLayer()) return;
		PhysicsLayer* physicsLayer = Application::Get().FindLayer<PhysicsLayer>();

		/// === Update Physics System ===
		/// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = 1;
		// TODO: Defer the collision update for stability.
		m_PhysicsSystem.Update(ts.GetSeconds(), cCollisionSteps, &physicsLayer->m_TempAllocator, &physicsLayer->m_JobSystem);

		each<RigidbodyComponent, TransformComponent>( MultiThreading::ExecutionPolicy::Parallel, [this](entt::entity e, RigidbodyComponent& rc, TransformComponent& trans) {
			if(!rc.m_Body) return;
			trans.SetPosition(Convert(rc.m_Body->GetPosition()));
			trans.SetRotation(Convert(rc.m_Body->GetRotation()));
		});
	}

	void Scene::StopPhysics()
	{
		VXM_PROFILE_FUNCTION();
		if(!CheckHasPhysicsLayer()) return;

		each<RigidbodyComponent>(MultiThreading::ExecutionPolicy::Parallel, [this](entt::entity, RigidbodyComponent& rc) {
			rc.m_Body = nullptr;
		});
	}

	void Scene::UpdatePhysicsState()
	{
		VXM_PROFILE_FUNCTION();
		if(!CheckHasPhysicsLayer()) return;

		/// === Update the position of the Rigidbody Already Created ===
		if(!IsStarted()) {
			each<TransformComponent, RigidbodyComponent>(exclude<DisableComponent>, [this](entt::entity e, TransformComponent& trans,  RigidbodyComponent& rc) {
				if(!rc.m_BodyID.IsInvalid()) {
					m_BodyInterface->SetPositionAndRotation(rc.m_BodyID, Convert(trans.GetPosition()), Convert(trans.GetRotation()), rc.GetActivation());
				}
			});
		}

		/// === Create New Rigidbody===
		each<RigidbodyComponent>(exclude<RigidbodyInitialized, DisableComponent>, [this](entt::entity e, RigidbodyComponent& rb) {CreatePhysicsBody(e,rb);});


		/// === Update Dirty Rigidbody ===
		auto viewRb = m_Registry.view<RigidbodyComponent, RigidbodyDirty>(exclude<DisableComponent>);
		std::for_each(viewRb.begin(), viewRb.end(), [this, &viewRb](auto e)
		{
		  auto& rb = viewRb.template get<RigidbodyComponent>(e);
		  UpdatePhysicsBody(e,rb);
		  Entity ent(e,this);
		  if(ent.HasComponent<RigidbodyDirty>()) ent.RemoveComponent<RigidbodyDirty>();
		});

		/// === Update Dirty Collider ===
		auto viewCol = m_Registry.view<RigidbodyComponent, ColliderDirty>(exclude<DisableComponent>);
		std::for_each(viewCol.begin(), viewCol.end(), [this, &viewCol](auto e)
		{
		  auto& rb = viewCol.template get<RigidbodyComponent>(e);
		  UpdateShape(e,rb);
		  Entity ent(e,this);
		  if(ent.HasComponent<ColliderDirty>()) ent.RemoveComponent<ColliderDirty>();
		});

		/// === Destroy Disabled Rigidbody ===
		auto toDestroyView = m_Registry.view<RigidbodyComponent, DisableComponent, RigidbodyInitialized>();
		std::for_each(toDestroyView.begin(), toDestroyView.end(), [this, &toDestroyView](auto e)
		{
		  RigidbodyComponent& rc = toDestroyView.template get<RigidbodyComponent>(e);
		  DestroyPhysicsBody(e,rc);
		});
	}

	const JPH::Shape* Scene::GetShape(Entity entity)
	{
		const JPH::Shape* shp{nullptr};
		if(entity.HasComponent<MeshColliderComponent>()) shp = entity.GetComponent<MeshColliderComponent>().GetShape();
		else if(entity.HasComponent<HeightFieldColliderComponent>()) shp = entity.GetComponent<HeightFieldColliderComponent>().GetShape();
		else if(entity.HasComponent<ColliderComponent>()) shp = entity.GetComponent<ColliderComponent>().GetShape();
		return shp;
	}

	void Scene::CreatePhysicsBody(entt::entity e, RigidbodyComponent &rb)
	{
		if(!rb.m_BodyID.IsInvalid()) return;

		Entity entity(e, this);
		const JPH::Shape* shp = GetShape(entity);
		if(!shp) return;

		auto creationSettings = rb.GetCreationSettings(shp, entity);
		auto activation = rb.GetActivation();
		rb.m_BodyID = m_BodyInterface->CreateAndAddBody(creationSettings, activation);

		m_BodyInterface->SetLinearAndAngularVelocity(rb.m_BodyID, Convert(rb.GetLinearVelocity()), Convert(rb.GetRadAngularVelocity()));
		m_BodyInterface->SetFriction(rb.m_BodyID, rb.GetFriction());
		m_BodyInterface->SetGravityFactor(rb.m_BodyID, rb.GetGravityFactor());

		entity.EnsureHasEmptyComponent<RigidbodyInitialized>();
	}

	void Scene::UpdatePhysicsBody(entt::entity e, RigidbodyComponent &rb)
	{
		if(rb.m_BodyID.IsInvalid()) {
			CreatePhysicsBody(e, rb);
		} else {
			auto activation = rb.GetActivation();
			if(!IsStarted()) m_BodyInterface->SetLinearAndAngularVelocity(rb.m_BodyID, Convert(rb.GetLinearVelocity()), Convert(rb.GetRadAngularVelocity()));
			m_BodyInterface->SetFriction(rb.m_BodyID, rb.GetFriction());
			m_BodyInterface->SetGravityFactor(rb.m_BodyID, rb.GetGravityFactor());
			m_BodyInterface->SetMotionType(rb.m_BodyID, rb.GetMotionType(), activation);
			m_BodyInterface->SetObjectLayer(rb.m_BodyID, rb.GetLayer());
		}
	}

	void Scene::UpdateShape(entt::entity e, RigidbodyComponent &rb)
	{
		if(rb.m_BodyID.IsInvalid()) {
			CreatePhysicsBody(e, rb);
		} else {
			const JPH::Shape* shp = GetShape({e, this});
			if(!shp) return;
			m_BodyInterface->SetShape(rb.m_BodyID, shp, true, rb.GetActivation());
		}
	}

	void Scene::DestroyPhysicsBody(entt::entity e, RigidbodyComponent &rc)
	{
		rc.m_Body = nullptr;
		if(!rc.m_BodyID.IsInvalid()) {
			m_BodyInterface->DestroyBody(rc.m_BodyID);
			rc.m_BodyID = JPH::BodyID();
		}
		Entity entity(e, this);
		if(entity.HasComponent<RigidbodyInitialized>()) entity.RemoveComponent<RigidbodyInitialized>();
		if(entity.HasComponent<ColliderDirty>()) entity.RemoveComponent<ColliderDirty>();
		if(entity.HasComponent<RigidbodyDirty>()) entity.RemoveComponent<RigidbodyDirty>();
	}

	bool Scene::CheckHasPhysicsLayer()
	{
		PhysicsLayer* physicsLayer = Application::Get().FindLayer<PhysicsLayer>();
		if(!physicsLayer) {VXM_CORE_ERROR("No Physics Layer in the application."); return false; }
		else return true;
	}
} // Voxymore
// Core