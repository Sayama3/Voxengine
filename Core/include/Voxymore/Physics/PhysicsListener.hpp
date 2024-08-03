//
// Created by ianpo on 31/07/2024.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"

#include <Jolt/Jolt.h>
// Jolt includes
//#include <Jolt/Core/Factory.h>
//#include <Jolt/Core/JobSystemThreadPool.h>
//#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
//#include <Jolt/Physics/Body/BodyCreationSettings.h>
//#include <Jolt/Physics/Collision/Shape/BoxShape.h>
//#include <Jolt/Physics/Collision/Shape/SphereShape.h>
//#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
//#include <Jolt/RegisterTypes.h>

namespace Voxymore::Core
{

	// An example contact listener
	class LoggerContactListener : public JPH::ContactListener
	{
	public:
		// See: ContactListener
		virtual JPH::ValidateResult OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override
		{
			VXM_CORE_INFO("Contact validate callback");

			// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
			return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
		}

		virtual void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
		{
			VXM_CORE_INFO("A contact was added");
		}

		virtual void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
		{
			VXM_CORE_INFO("A contact was persisted");
		}

		virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override
		{
			VXM_CORE_INFO("A contact was removed");
		}
	};

	// An example activation listener
	class LoggerActivationListener : public JPH::BodyActivationListener
	{
	public:
		virtual void OnBodyActivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override
		{
			VXM_CORE_INFO("A body got activated");
		}

		virtual void OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override
		{
			VXM_CORE_INFO("A body went to sleep");
		}
	};

} // namespace Voxymore::Core

