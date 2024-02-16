//
// Created by ianpo on 18/01/2024.
//

#pragma once

#include "Voxymore/ParticlePhysic/Components/FloatingComponent.hpp"
#include "Voxymore/ParticlePhysic/Components/ParticleComponent.hpp"
#include "Voxymore/Scene/Systems.hpp"
#include "static_block.hpp"

#define VXM_DefaultWaterHeight 0.0
#define VXM_DefaultLiquidDensity 1000.0

namespace Voxymore::Core
{

	class BuoyancyForceSystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(BuoyancyForceSystem);
	public:
		BuoyancyForceSystem() = default;
		~BuoyancyForceSystem() = default;
	protected:
		virtual void DeserializeSystem(YAML::Node& node) override;
		virtual void SerializeSystem(YAML::Emitter& out) override;
		virtual void ResetSystem() override;
	public:
		virtual bool OnImGuiRender() override;
		virtual void Update(Scene& scene, TimeStep ts) override;

		inline virtual bool RunOnAllScenes() override { return true; }
	private:
		void UpdateParticle(entt::entity e, ParticleComponent& pc, FloatingComponent& fc, TransformComponent& tc);


	/**
	 * The height (on the global Y axis) of the water of this object.
	 */
	Real m_WaterHeight = VXM_DefaultWaterHeight;

	/**
	 * The density of the liquid. Pure water has a density of 1000 (kg/m3).
	 */
	Real m_LiquidDensity = VXM_DefaultLiquidDensity;
	};

	static_block{ BuoyancyForceSystem::CreateSystem(); };

} // namespace Voxymore::Core

