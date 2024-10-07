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
//#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
//#include <Jolt/Physics/Collision/Shape/BoxShape.h>
//#include <Jolt/Physics/Collision/Shape/SphereShape.h>
//#include <Jolt/Physics/PhysicsSettings.h>
//#include <Jolt/Physics/PhysicsSystem.h>
//#include <Jolt/RegisterTypes.h>

namespace Voxymore::Core
{

	// Layer that objects can be in, determines which other objects it can collide with
	// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
	// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
	// but only if you do collision testing).
	namespace PhysicalLayers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};

	/// Class that determines if two object layers can collide
	class ObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};

} // namespace Voxymore::Core

