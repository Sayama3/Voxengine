//
// Created by ianpo on 31/07/2024.
//

#include "Voxymore/Physics/BroadPhaseLayerInterface.hpp"

namespace Voxymore::Core
{


	BroadPhaseLayerInterface::BroadPhaseLayerInterface()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[ PhysicalLayers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[ PhysicalLayers::MOVING] = BroadPhaseLayers::MOVING;
	}

	JPH::uint BroadPhaseLayerInterface::GetNumBroadPhaseLayers() const
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer BroadPhaseLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
	{
		VXM_CORE_ASSERT(inLayer < PhysicalLayers::NUM_LAYERS, "The inLayer {} is not valid. Max Layer = {}", inLayer, PhysicalLayers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char *BroadPhaseLayerInterface::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
			case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
			case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
			default: VXM_CORE_ASSERT(false, "The layer {} is invalid.", inLayer.GetValue()); return "INVALID";
		}
	}
#endif

}// namespace Voxymore::Core
