//
// Created by ianpo on 19/02/2024.
//

#pragma once

#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyFloatingComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/Scene/Systems.hpp"

#ifndef VXM_SYSTEM_DEFAULT_WATER_HEIGHT
#define VXM_SYSTEM_DEFAULT_WATER_HEIGHT 0.0
#endif

#ifndef VXM_SYSTEM_DEFAULT_LIQUID_DENSITY
#define VXM_SYSTEM_DEFAULT_LIQUID_DENSITY 1000
#endif

namespace Voxymore::Core
{

	class RigidbodyBuoyancySystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(RigidbodyBuoyancySystem);
	public:
		RigidbodyBuoyancySystem() = default;
		~RigidbodyBuoyancySystem() = default;
	protected:
		virtual void DeserializeSystem(YAML::Node& node) override;
		virtual void SerializeSystem(YAML::Emitter& out) override;
		virtual void ResetSystem() override;
	public:
		virtual bool OnImGuiRender() override;
		virtual void Update(Scene& scene, TimeStep ts) override;
	private:
		void UpdateParticle(entt::entity e, RigidbodyComponent& pc, TransformComponent& tc, RigidbodyFloatingComponent& rfc);

		/**
		* The height (on the global Y axis) of the water of this object.
		*/
		Real m_WaterHeight = VXM_SYSTEM_DEFAULT_WATER_HEIGHT;

		/**
		* The density of the liquid. Pure water has a density of 1000 (kg/m3).
		*/
		Real m_LiquidDensity = VXM_SYSTEM_DEFAULT_LIQUID_DENSITY;
	};

} // namespace Voxymore::Core

