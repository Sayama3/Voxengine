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

namespace Voxymore::Core
{

	class FloatingComponent : Component
	{
	VXM_IMPLEMENT_COMPONENT(FloatingComponent);
	public:
		FloatingComponent() = default;
		~FloatingComponent() = default;

		void DeserializeComponent(YAML::Node& node) override;
		void SerializeComponent(YAML::Emitter& out) override;
		bool OnImGuiRender() override;

		/**
		 * The maximum submersion depth of an object before it generates its maximum buoyancy force.
		 */
		Real MaxDepth = 0;

		/**
		 * The volume of the object
		 */
		Real Volume = 1;

		/**
		 * The height (on the global Y axis) of the water of this object.
		 * Can be override to use this one instead of the global one.
		 */
		std::optional<Real> WaterHeight = {};

		/**
		 * The density of the liquid. Pure water has a density of 1000 (kg/m3)
		 * Can be override to use this one instead of the global one.
		 */
		std::optional<Real> LiquidDensity = {};


	};

	static_block{ FloatingComponent fc; };
} // namespace Voxymore::Core

