//
// Created by ianpo on 05/01/2024.
//

#include <utility>

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Physics/Components/ParticleComponent.hpp"
#include "Voxymore/Physics/PhysicsLayer.hpp"
#include "Voxymore/Physics/Systems/SpringForceSystem.hpp"
#include "Voxymore/Physics/Systems/BuoyancyForceSystem.hpp"


namespace Voxymore::Core
{
	PhysicsLayer::PhysicsLayer() : Layer("PhysicsLayer")
	{
		std::cout << SpringForceSystem::GetInstance()->GetName() << std::endl;
		std::cout << BuoyancyForceSystem::GetInstance()->GetName() << std::endl;
	}

	PhysicsLayer::~PhysicsLayer()
	{
	}

	void PhysicsLayer::OnUpdate(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();

		if(!HasScene()) {
			return;
		}

		if(!m_SceneHandle->IsStarted()) {
			return;
		}

		// Integrate all Particles
		auto func = [&ts](entt::entity e, ParticleComponent& pc, TransformComponent& tc){
			VXM_PROFILE_FUNCTION();
			pc.SetPosition(tc.GetPosition());
			pc.Integrate(ts);
			tc.SetPosition(pc.GetPosition());
		};

		m_SceneHandle->each<ParticleComponent, TransformComponent>(exclude<DisableComponent>, func);

		if(!m_Contacts.empty())
		{
			VXM_CORE_INFO("Resolve {0} contacts with maximum {1} iterations.",m_Contacts.size() , m_Contacts.size() * 2);
			m_Resolver.SetIterations(m_Contacts.size() * 2);
			m_Resolver.ResolveContacts(ts, m_Contacts);
			m_Contacts.clear();
		}
	}

	void PhysicsLayer::SetScene(Ref<Scene> scene)
	{
		VXM_PROFILE_FUNCTION();
		m_SceneHandle = std::move(scene);
	}

	void PhysicsLayer::ResetScene()
	{
		VXM_PROFILE_FUNCTION();
		m_SceneHandle = nullptr;
	}

	bool PhysicsLayer::HasScene() const
	{
		VXM_PROFILE_FUNCTION();
		return m_SceneHandle != nullptr;
	}

	const Vec3& PhysicsLayer::GetGravity() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Gravity;
	}
	void PhysicsLayer::SetGravity(const Vec3& g)
	{
		VXM_PROFILE_FUNCTION();
		m_Gravity = g;
	}
	void PhysicsLayer::AddContact(const ParticleContact& contact)
	{
		VXM_PROFILE_FUNCTION();
		m_Contacts.push_back(contact);
	}

	void PhysicsLayer::AddContacts(const std::vector<ParticleContact>& contacts)
	{
		VXM_PROFILE_FUNCTION();
		m_Contacts.insert(m_Contacts.end(), contacts.begin(), contacts.end());
	}
} // namespace Voxymore::Core
