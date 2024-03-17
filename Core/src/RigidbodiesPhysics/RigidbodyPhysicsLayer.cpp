//
// Created by ianpo on 05/01/2024.
//

#include <utility>

#include "Voxymore/Core/TypeHelpers.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.decl.hpp"
#include "Voxymore/Math/BoundingSphere.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyFloatingComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodySpringComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/ColliderComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/RigidbodyPhysicsLayer.hpp"
#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodyBuoyancySystem.hpp"
#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodySpringSystem.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/BroadCollisions.hpp"
#include "Voxymore/RigidbodiesPhysics/Collisions/CollisionDetector.hpp"


namespace Voxymore::Core
{
	RigidbodyPhysicsLayer::RigidbodyPhysicsLayer() : Layer("RigidbodyPhysicsLayer"), m_Resolver(0), m_Root(new BVHNode<BoundingSphere>(nullptr, nullptr, BoundingSphere()))
	{
	}

	RigidbodyPhysicsLayer::~RigidbodyPhysicsLayer()
	{
		delete m_Root;
	}

	void RigidbodyPhysicsLayer::OnAttach()
	{
		VXM_PROFILE_FUNCTION();
		RigidbodySpringSystem::CreateSystem();
		RigidbodyBuoyancySystem::CreateSystem();
		ColliderComponent::RegisterComponent();
		RigidbodyComponent::RegisterComponent();
		RigidbodySpringComponent::RegisterComponent();
		RigidbodyFloatingComponent::RegisterComponent();
	}

	void RigidbodyPhysicsLayer::OnDetach()
	{
		VXM_PROFILE_FUNCTION();
		RigidbodySpringSystem::DeleteSystem();
		RigidbodyBuoyancySystem::DeleteSystem();
		ColliderComponent::UnregisterComponent();
		RigidbodyComponent::UnregisterComponent();
		RigidbodySpringComponent::UnregisterComponent();
		RigidbodyFloatingComponent::UnregisterComponent();
	}

	void RigidbodyPhysicsLayer::OnUpdate(TimeStep ts)
	{
		//TODO: Use a Coherence System to handle object not moving on a ground.

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

		m_SceneHandle->each<RigidbodyComponent, TransformComponent>(exclude<DisableComponent, DisableRigidbody>,  MultiThreading::ExecutionPolicy::Parallel, func);

		delete m_Root;
		m_Root = nullptr;
		auto view = m_SceneHandle->view<RigidbodyComponent, ColliderComponent, TransformComponent>(exclude<DisableComponent, DisableRigidbody>);
		for (auto e : view)
		{
			RigidbodyComponent& rc = view.get<RigidbodyComponent>(e);
			ColliderComponent& cc = view.get<ColliderComponent>(e);
			TransformComponent& tc = view.get<TransformComponent>(e);
			if(m_Root) m_Root->Insert(reinterpret_cast<Rigidbody*>(&rc), Entity(e,m_SceneHandle.get()), cc.GetBoundingSphere());
			else m_Root = new BVHNode<BoundingSphere>(nullptr, reinterpret_cast<Rigidbody*>(&rc), Entity(e,m_SceneHandle.get()), cc.GetBoundingSphere());
		}

		if(!m_Root)
		{
			VXM_CORE_WARN("No rigidbodies found in the scene.");
			return;
		}

		m_PotentialContacts.reserve(view.size_hint());
		uint32_t count = m_Root->GetPotentialContacts(m_PotentialContacts);
		m_Contacts.reserve(count);

		CollisionData* contacts = &m_Contacts;

		for (PotentialContact& potentialContact : m_PotentialContacts)
		{
			auto&& [body0, entity0] = potentialContact.bodies[0];
			auto& col0 = entity0.GetComponent<ColliderComponent>();

			auto&& [body1, entity1] = potentialContact.bodies[1];
			auto& col1 = entity1.GetComponent<ColliderComponent>();

			auto bb = [&contacts](Box& one, Box& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto bs = [&contacts](Box& one, Sphere& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto bp = [&contacts](Box& one, Plane& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto sb = [&contacts](Sphere& one, Box& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto ss = [&contacts](Sphere& one, Sphere& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto sp = [&contacts](Sphere& one, Plane& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto pb = [&contacts](Plane& one, Box& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto ps = [&contacts](Plane& one, Sphere& two){ return CollisionDetector::Collide(one,two,contacts);};
			auto pp = [](Plane& one, Plane& two){ return 0u;};

			auto collisionPoints = std::visit(overloads{bb, bs, bp, sb, ss, sp, pb, ps, pp}, col0.m_Collider, col1.m_Collider);
		}
		m_PotentialContacts.clear();

		if(!m_Contacts.empty())
		{
			VXM_CORE_INFO("Resolve {0} contacts with maximum {1} iterations.",m_Contacts.size() , m_Contacts.size() * 2);
//			m_Resolver.SetIterations(m_Contacts.size() * 2);
			m_Resolver.ResolveContacts(ts, m_Contacts.contacts);
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
		m_SceneHandle = nullptr;
	}

	bool RigidbodyPhysicsLayer::HasScene() const
	{
		return m_SceneHandle != nullptr;
	}

	const Vec3&RigidbodyPhysicsLayer::GetGravity() const
	{
		return m_Gravity;
	}
	void RigidbodyPhysicsLayer::SetGravity(const Vec3& g)
	{
		m_Gravity = g;
	}
	void RigidbodyPhysicsLayer::AddContact(const RigidbodyContact& contact)
	{
		VXM_PROFILE_FUNCTION();
		m_Contacts.contacts.push_back(contact);
	}

	void RigidbodyPhysicsLayer::AddContacts(const std::vector<RigidbodyContact>& contacts)
	{
		VXM_PROFILE_FUNCTION();
		m_Contacts.contacts.insert(m_Contacts.end(), contacts.begin(), contacts.end());
	}
} // namespace Voxymore::Core
