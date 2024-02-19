//
// Created by ianpo on 19/02/2024.
//

#pragma once

#include "Voxymore/Core/Math.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/Components/CustomComponent.hpp"

#ifndef VXM_DEFAULT_LOCAL_POSITION
#define VXM_DEFAULT_LOCAL_POSITION {0,0,0};
#endif
#ifndef VXM_DEFAULT_MAX_DEPTH
#define VXM_DEFAULT_MAX_DEPTH 0.5
#endif
#ifndef VXM_DEFAULT_VOLUME
#define VXM_DEFAULT_VOLUME 0.015
#endif
#ifndef VXM_DEFAULT_WATER_HEIGHT
#define VXM_DEFAULT_WATER_HEIGHT {}
#endif
#ifndef VXM_DEFAULT_LIQUID_DENSITY
#define VXM_DEFAULT_LIQUID_DENSITY {}
#endif

namespace Voxymore::Core
{

	class RigidbodyFloatingComponent : public Component<RigidbodyFloatingComponent>
	{
		VXM_IMPLEMENT_COMPONENT(RigidbodyFloatingComponent);
	public:
		RigidbodyFloatingComponent() = default;
		~RigidbodyFloatingComponent() = default;
	public:
		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();
	public:

		Vec3 LocalPosition = VXM_DEFAULT_LOCAL_POSITION;

		/**
		 * The height (on the global Y axis) of the water of this object.
		 * Can be override to use this one instead of the global one.
		 */
		std::optional<Real> WaterHeight = VXM_DEFAULT_WATER_HEIGHT;

		/**
		 * The density of the liquid. Pure water has a density of 1000 (kg/m3)
		 * Can be override to use this one instead of the global one.
		 */
		std::optional<Real> LiquidDensity = VXM_DEFAULT_LIQUID_DENSITY;

		/**
		 * The maximum submersion depth of an object before it generates its maximum buoyancy force.
		 */
		Real MaxDepth = VXM_DEFAULT_MAX_DEPTH;

		/**
		 * The volume of the object
		 */
		Real Volume = VXM_DEFAULT_VOLUME;
	};

} // namespace Voxymore::Core

