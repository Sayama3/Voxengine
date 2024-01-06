//
// Created by ianpo on 05/01/2024.
//

#include <utility>

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Physics/Components/ParticleComponent.hpp"
#include "Voxymore/Physics/PhysicsLayer.hpp"
#include "Voxymore/Physics/Systems/GravitySystem.hpp"
#include "Voxymore/Physics/Systems/ParticleDragSystem.hpp"
#include "Voxymore/Physics/Systems/SpringForceSystem.hpp"


namespace Voxymore::Core
{
	PhysicsLayer::PhysicsLayer() : Layer("PhysicsLayer")
	{
		std::cout << GravitySystem::GetInstance()->GetName() << std::endl;
		std::cout << ParticleDragSystem::GetInstance()->GetName() << std::endl;
		std::cout << SpringForceSystem::GetInstance()->GetName() << std::endl;
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
		m_SceneHandle->each<ParticleComponent, TransformComponent>(exclude<DisableComponent>, [&](ParticleComponent& pc, TransformComponent& tc){
			VXM_PROFILE_FUNCTION();
			pc.SetPosition(tc.GetPosition());
			pc.Integrate(ts);
			tc.SetPosition(pc.GetPosition());
		});
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
} // namespace Voxymore::Core
