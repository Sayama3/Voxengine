//
// Created by ianpo on 05/01/2024.
//

#include <utility>
#include <algorithm>

#include "Voxymore/Core/TypeHelpers.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.decl.hpp"
#include "Voxymore/RigidbodiesPhysics/RigidbodyPhysicsLayer.hpp"

#include <Jolt/Jolt.h>
// Jolt includes
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>



namespace Voxymore::Core
{
	RigidbodyPhysicsLayer::RigidbodyPhysicsLayer() : Layer("Rigidbody Physics Layer")
	{
	}

	RigidbodyPhysicsLayer::~RigidbodyPhysicsLayer() = default;

	void RigidbodyPhysicsLayer::OnAttach()
	{
		VXM_PROFILE_FUNCTION();

		// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
		// It is not directly used in this example but still required.
		JPH::Factory::sInstance = new JPH::Factory();

		// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
		// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
		// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
		JPH::RegisterTypes();
	}

	void RigidbodyPhysicsLayer::OnDetach()
	{
		VXM_PROFILE_FUNCTION();

		// Unregisters all types with the factory and cleans up the default material
		JPH::UnregisterTypes();

		// Destroy the factory
		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
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
	}

	void RigidbodyPhysicsLayer::SetScene(Ref<Scene> scene)
	{
		VXM_PROFILE_FUNCTION();
		m_SceneHandle = std::move(scene);
	}

	void RigidbodyPhysicsLayer::ResetScene()
	{
		m_SceneHandle = nullptr;
	}

	bool RigidbodyPhysicsLayer::HasScene() const
	{
		return m_SceneHandle != nullptr;
	}
} // namespace Voxymore::Core
