//
// Created by ianpo on 31/07/2024.
//

#pragma once

#include "ObjectLayerPairFilter.hpp"
#include "BroadPhaseLayerInterface.hpp"


namespace Voxymore::Core
{

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
		{
			switch (inLayer1)
			{
				case PhysicalLayers::NON_MOVING:
					return inLayer2 == BroadPhaseLayers::MOVING;
				case PhysicalLayers::MOVING:
					return true;
				default:
					JPH_ASSERT(false);
					return false;
			}
		}
	};

} // namespace Voxymore::Core

