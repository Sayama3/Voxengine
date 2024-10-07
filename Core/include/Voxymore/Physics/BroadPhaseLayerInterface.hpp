//
// Created by ianpo on 31/07/2024.
//

#pragma once


#include "ObjectLayerPairFilter.hpp"
#include <Jolt/Jolt.h>

// Jolt includes
//#include <Jolt/Core/Factory.h>
//#include <Jolt/Core/JobSystemThreadPool.h>
//#include <Jolt/Core/TempAllocator.h>
//#include <Jolt/Physics/Body/BodyActivationListener.h>
//#include <Jolt/Physics/Body/BodyCreationSettings.h>
//#include <Jolt/Physics/Collision/Shape/BoxShape.h>
//#include <Jolt/Physics/Collision/Shape/SphereShape.h>
//#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
//#include <Jolt/RegisterTypes.h>

namespace Voxymore::Core
{
	// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
	// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
	// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
	// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
	// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr JPH::uint NUM_LAYERS(2);
	};

	// JPH::BroadPhaseLayerInterface implementation
	// This defines a mapping between object and broadphase layers.
	class BroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BroadPhaseLayerInterface();

		virtual JPH::uint GetNumBroadPhaseLayers() const override;

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char * GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer mObjectToBroadPhase[PhysicalLayers::NUM_LAYERS];
	};

} // namespace Voxymore::Core

