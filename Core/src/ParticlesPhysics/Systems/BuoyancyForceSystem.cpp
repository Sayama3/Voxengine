//
// Created by ianpo on 18/01/2024.
//

#include "Voxymore/Physics/Particles/Systems/BuoyancyForceSystem.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore::Core
{
	VXM_DECLARE_SYSTEM(BuoyancyForceSystem);

	void BuoyancyForceSystem::DeserializeSystem(YAML::Node& node)
	{
		VXM_PROFILE_FUNCTION();
		System::DeserializeSystem(node);

		auto waterHeightNode = node["WaterHeight"];
		if(waterHeightNode.IsDefined())
		{
			m_WaterHeight = waterHeightNode.as<Real>();
		}
		else
		{
			m_WaterHeight = VXM_DefaultWaterHeight;
		}

		auto liquidDensityNode = node["LiquidDensity"];
		if(liquidDensityNode.IsDefined())
		{
			m_LiquidDensity = liquidDensityNode.as<Real>();
		}
		else
		{
			m_LiquidDensity = VXM_DefaultLiquidDensity;
		}
	}

	void BuoyancyForceSystem::SerializeSystem(YAML::Emitter& out)
	{
		VXM_PROFILE_FUNCTION();
		System::SerializeSystem(out);

		out << KEYVAL("WaterHeight", m_WaterHeight);
		out << KEYVAL("LiquidDensity", m_LiquidDensity);
	}

	void BuoyancyForceSystem::ResetSystem()
	{
		VXM_PROFILE_FUNCTION();
		m_WaterHeight = VXM_DefaultWaterHeight;
		m_LiquidDensity = VXM_DefaultLiquidDensity;
	}

	bool BuoyancyForceSystem::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = System::OnImGuiRender();

		changed |= ImGuiLib::DragReal("Water Height", &m_WaterHeight);
		if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
		{
			ImGui::SetTooltip("The height (on the global Y axis) of the water of this object.");
		}

		changed |= ImGuiLib::DragReal("Liquid Density", &m_LiquidDensity);
		if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
		{
			ImGui::SetTooltip("The density of the liquid. Pure water has a density of 1000 (kg/m3).");
		}

		return changed;
	}

	void BuoyancyForceSystem::Update(Scene &scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		auto view = scene.view<ParticleComponent, FloatingComponent, TransformComponent>();
		auto func= VXM_BIND_EVENT_FN(UpdateParticle);
		scene.each<ParticleComponent, FloatingComponent, TransformComponent>(exclude<DisableComponent>, MultiThreading::ExecutionPolicy::Parallel_Unsequenced, func);
	}

	void BuoyancyForceSystem::UpdateParticle(entt::entity e, ParticleComponent& pc, FloatingComponent& fc, TransformComponent& tc)
	{
		VXM_PROFILE_FUNCTION();
		Real depth = tc.GetPosition().y;
		Real waterHeight = fc.WaterHeight.has_value() ? fc.WaterHeight.value() : m_WaterHeight;
		Real liquidDensity = fc.LiquidDensity.has_value() ? fc.LiquidDensity.value() : m_LiquidDensity;

		// Above water, don't touch any forces
		if(depth >= (waterHeight + fc.MaxDepth)) {
			return;
		}

		Vec3 force = Vec3(0,liquidDensity * fc.Volume,0);

		if(depth > waterHeight - fc.MaxDepth)
		{
			auto t = (-depth + fc.MaxDepth + waterHeight) / (fc.MaxDepth * 2.0);
			force.y *= t;
		}

		pc.AccumulateForce(force);
	}


}// namespace Vo