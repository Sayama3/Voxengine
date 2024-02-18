//
// Created by ianpo on 18/01/2024.
//

#pragma once

#include "static_block.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.hpp"

#define VXM_DEFAULT_MAX_DEPTH 0.5
#define VXM_DEFAULT_VOLUME 0.015
#define VXM_DEFAULT_WATER_HEIGHT {}
#define VXM_DEFAULT_LIQUID_DENSITY {}

namespace Voxymore::Core
{
	class FloatingComponent : public Component<FloatingComponent>
	{
		VXM_IMPLEMENT_COMPONENT(FloatingComponent);
	public:
		FloatingComponent() = default;
		~FloatingComponent() = default;

		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();

		/**
		 * The maximum submersion depth of an object before it generates its maximum buoyancy force.
		 */
		Real MaxDepth = VXM_DEFAULT_MAX_DEPTH;

		/**
		 * The volume of the object
		 */
		Real Volume = VXM_DEFAULT_VOLUME;

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


	};
} // namespace Voxymore::Core

