//
// Created by ianpo on 05/01/2024.
//

#include <utility>

#include "Voxymore/RigidbodiesPhysics/RigidbodyPhysicsLayer.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodySpringComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyFloatingComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodySpringSystem.hpp"
#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodyBuoyancySystem.hpp"


namespace Voxymore::Core
{
	RigidbodyPhysicsLayer::RigidbodyPhysicsLayer() : Layer("RigidbodyPhysicsLayer"), m_Resolver(0)
	{
	}

	RigidbodyPhysicsLayer::~RigidbodyPhysicsLayer()
	{
	}

	void RigidbodyPhysicsLayer::OnAttach()
	{
		VXM_PROFILE_FUNCTION();
		RigidbodySpringSystem::CreateSystem();
		RigidbodyBuoyancySystem::CreateSystem();
		RigidbodyComponent::RegisterComponent();
		RigidbodySpringComponent::RegisterComponent();
		RigidbodyFloatingComponent::RegisterComponent();
	}

	void RigidbodyPhysicsLayer::OnDetach()
	{
		VXM_PROFILE_FUNCTION();
		RigidbodySpringSystem::DeleteSystem();
		RigidbodyBuoyancySystem::DeleteSystem();
		RigidbodyComponent::UnregisterComponent();
		RigidbodySpringComponent::UnregisterComponent();
		RigidbodyFloatingComponent::UnregisterComponent();
	}

	void RigidbodyPhysicsLayer::OnUpdate(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();

		if(!HasScene()) {
			return;
		}

		if(!m_SceneHandle->IsStarted()) {
			return;
		}

		// Integrate all Rigidbodies
		auto func = [&ts](entt::entity e, RigidbodyComponent& pc, TransformComponent& tc){
			VXM_PROFILE_FUNCTION();

			pc.SetPosition(tc.GetPosition());
			pc.SetOrientation(tc.GetRotation());

			pc.Integrate(ts);

			tc.SetPosition(pc.GetPosition());
			tc.SetRotation(pc.GetOrientation());
		};

		m_SceneHandle->each<RigidbodyComponent, TransformComponent>(exclude<DisableComponent>,  MultiThreading::ExecutionPolicy::Parallel, func);

		if(!m_Contacts.empty())
		{
			VXM_CORE_INFO("Resolve {0} contacts with maximum {1} iterations.",m_Contacts.size() , m_Contacts.size() * 2);
			m_Resolver.SetIterations(m_Contacts.size() * 2);
			m_Resolver.ResolveContacts(ts, m_Contacts);
			m_Contacts.clear();
		}
	}

	void RigidbodyPhysicsLayer::SetScene(Ref<Scene> scene)
	{
		VXM_PROFILE_FUNCTION();
		m_SceneHandle = std::move(scene);
		// Integrate all Rigidbodies
		auto func = [](entt::entity e, RigidbodyComponent& pc, TransformComponent& tc){
			pc.SetPosition(tc.GetPosition());
			pc.SetOrientation(tc.GetRotation());
		};

		m_SceneHandle->each<RigidbodyComponent, TransformComponent>(exclude<DisableComponent>, MultiThreading::ExecutionPolicy::Parallel, func);
	}

	void RigidbodyPhysicsLayer::ResetScene()
	{
		VXM_PROFILE_FUNCTION();
		m_SceneHandle = nullptr;
	}

	bool RigidbodyPhysicsLayer::HasScene() const
	{
		VXM_PROFILE_FUNCTION();
		return m_SceneHandle != nullptr;
	}

	const Vec3&RigidbodyPhysicsLayer::GetGravity() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Gravity;
	}
	void RigidbodyPhysicsLayer::SetGravity(const Vec3& g)
	{
		VXM_PROFILE_FUNCTION();
		m_Gravity = g;
	}
	void RigidbodyPhysicsLayer::AddContact(const RigidbodyContact& contact)
	{
		VXM_PROFILE_FUNCTION();
		m_Contacts.push_back(contact);
	}

	void RigidbodyPhysicsLayer::AddContacts(const std::vector<RigidbodyContact>& contacts)
	{
		VXM_PROFILE_FUNCTION();
		m_Contacts.insert(m_Contacts.end(), contacts.begin(), contacts.end());
	}
} // namespace Voxymore::Core
