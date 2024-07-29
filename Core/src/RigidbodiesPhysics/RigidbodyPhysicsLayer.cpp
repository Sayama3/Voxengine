//
// Created by ianpo on 05/01/2024.
//

#include <utility>
#include <algorithm>

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

		Integrate(ts);

		if(!BroadCollisionCheck(ts)) return;

		if(!FineCollisionCheck(ts)) return;

		// Filter False Positive
		std::erase_if(m_Contacts.contacts, [](RigidbodyContact rc) -> bool {
			if(Math::Approx0(Math::SqrMagnitude(rc.contactNormal), 0.001)) return true;
			if(Math::Approx0(rc.penetration, 0.001)) return true;
			return false;
		});

		CollisionResolution(ts);
	}

	void RigidbodyPhysicsLayer::Integrate(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();

		// Integrate all Rigidbodies
		auto func = [&ts](entt::entity e, RigidbodyComponent& rc, TransformComponent& tc){
			rc.SetTransform(&tc);
			rc.Integrate(ts);
		};

		m_SceneHandle->each<RigidbodyComponent, TransformComponent>(exclude<DisableComponent, DisableRigidbody>,  MultiThreading::ExecutionPolicy::Parallel, func);
	}

	bool RigidbodyPhysicsLayer::BroadCollisionCheck(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		delete m_Root;
		m_Root = nullptr;
		auto view = m_SceneHandle->view<ColliderComponent, TransformComponent>(exclude<DisableComponent>);
		for (auto e : view)
		{
			Entity entity(e, m_SceneHandle.get());
			RigidbodyComponent* rc = entity.HasComponent<RigidbodyComponent>() ? &entity.GetComponent<RigidbodyComponent>() : nullptr;
			TransformComponent& tc = view.get<TransformComponent>(e);
			ColliderComponent& cc = view.get<ColliderComponent>(e);
			cc.SetTransform(&tc);
			cc.SetRigidbody(rc);
			if(m_Root) m_Root->Insert(reinterpret_cast<Rigidbody*>(&rc), Entity(e,m_SceneHandle.get()), cc.GetBoundingSphere());
			else m_Root = new BVHNode<BoundingSphere>(nullptr, reinterpret_cast<Rigidbody*>(&rc), Entity(e,m_SceneHandle.get()), cc.GetBoundingSphere());
		}

		if(!m_Root)
		{
			VXM_CORE_WARN("No rigidbodies found in the scene.");
			return false;
		}

		m_PotentialContacts.clear();
		m_PotentialContacts.reserve(view.size_hint());
		m_Root->GetPotentialContacts(m_PotentialContacts);
		return !m_PotentialContacts.empty();
	}

	bool RigidbodyPhysicsLayer::FineCollisionCheck(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		m_Contacts.clear();
		m_Contacts.reserve(m_PotentialContacts.size());
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
			auto pp = [](Plane& one, Plane& two){ return uint32_t(0u);};

			auto collisionPoints = std::visit(overloads{bb, bs, bp, sb, ss, sp, pb, ps, pp}, col0.m_Collider, col1.m_Collider);
		}

		return !m_Contacts.empty();
	}

	void RigidbodyPhysicsLayer::CollisionResolution(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		if(!m_Contacts.empty())
		{
			VXM_CORE_INFO("Resolve {0} contacts with maximum {1} m_Iterations.",m_Contacts.size() , m_Contacts.size() * 2);
			m_Resolver.SetIterations(m_Contacts.size() * 2);
			m_Resolver.PrepareContacts(ts, m_Contacts.contacts);
			// Second Filter False Positive
			std::erase_if(m_Contacts.contacts, [](RigidbodyContact rc) -> bool {
				auto cv = rc.GetContactVelocity();
				auto ddv = rc.GetDesiredDeltaVelocity();
				if(Math::Approx0(Math::Magnitude(cv)) && Math::Approx0(ddv)) return true;
				return false;
			});
			if(!m_Contacts.empty()) {
				m_Resolver.ResolveContacts(ts, m_Contacts.contacts);
			}
		}
	}

	void RigidbodyPhysicsLayer::SetScene(Ref<Scene> scene)
	{
		VXM_PROFILE_FUNCTION();
		m_SceneHandle = std::move(scene);
		// Integrate all Rigidbodies
		auto func0 = [](entt::entity e, RigidbodyComponent& rc, TransformComponent& tc){
			rc.SetTransform(&tc);
		};
		auto func1 = [](entt::entity e, RigidbodyComponent& rc, ColliderComponent& cc){
			cc.SetRigidbody(&rc);
		};
		auto func2 = [](entt::entity e, ColliderComponent& cc, TransformComponent& tc){
			cc.SetTransform(&tc);
		};

		m_SceneHandle->each<RigidbodyComponent, TransformComponent>(MultiThreading::ExecutionPolicy::Parallel, func0);
		m_SceneHandle->each<RigidbodyComponent, ColliderComponent>(MultiThreading::ExecutionPolicy::Parallel, func1);
		m_SceneHandle->each<ColliderComponent, TransformComponent>(MultiThreading::ExecutionPolicy::Parallel, func2);
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
