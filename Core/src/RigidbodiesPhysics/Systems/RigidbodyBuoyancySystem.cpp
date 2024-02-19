//
// Created by ianpo on 19/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodyBuoyancySystem.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"


namespace Voxymore::Core
{
	VXM_DECLARE_SYSTEM(RigidbodyBuoyancySystem);
	void RigidbodyBuoyancySystem::DeserializeSystem(YAML::Node& node)
	{
		VXM_PROFILE_FUNCTION();
		if(node["WaterHeight"].IsDefined())
		{
			m_WaterHeight = node["WaterHeight"].as<Real>();
		}
		else
		{
			m_WaterHeight = VXM_SYSTEM_DEFAULT_WATER_HEIGHT;
		}
		if(node["LiquidDensity"].IsDefined())
		{
			m_LiquidDensity = node["LiquidDensity"].as<Real>();
		}
		else
		{
			m_LiquidDensity = VXM_SYSTEM_DEFAULT_LIQUID_DENSITY;
		}
	}

	void RigidbodyBuoyancySystem::SerializeSystem(YAML::Emitter& out)
	{
		VXM_PROFILE_FUNCTION();
		out << KEYVAL("WaterHeight", m_WaterHeight);
		out << KEYVAL("LiquidDensity", m_LiquidDensity);
	}

	void RigidbodyBuoyancySystem::ResetSystem()
	{
		VXM_PROFILE_FUNCTION();
		m_WaterHeight = VXM_SYSTEM_DEFAULT_WATER_HEIGHT;
		m_LiquidDensity = VXM_SYSTEM_DEFAULT_LIQUID_DENSITY;
	}

	bool RigidbodyBuoyancySystem::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;
		changed |= ImGuiLib::DragReal("WaterHeight", &m_WaterHeight);
		changed |= ImGuiLib::DragReal("LiquidDensity", &m_LiquidDensity);
		return changed;
	}

	void RigidbodyBuoyancySystem::Update(Scene& scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		auto func = VXM_BIND_EVENT_FN(UpdateParticle);
		scene.each<RigidbodyComponent, TransformComponent, RigidbodyFloatingComponent>(exclude<DisableComponent>, MultiThreading::ExecutionPolicy::Parallel, func);
	}

	void RigidbodyBuoyancySystem::UpdateParticle(entt::entity e, RigidbodyComponent& pc, TransformComponent& tc, RigidbodyFloatingComponent&rfc)
	{
		VXM_PROFILE_FUNCTION();

		auto worldPoint = tc.GetWorldPoint(rfc.LocalPosition);
		Real depth = worldPoint.y;
		Real waterHeight = rfc.WaterHeight.has_value() ? rfc.WaterHeight.value() : m_WaterHeight;
		Real liquidDensity = rfc.LiquidDensity.has_value() ? rfc.LiquidDensity.value() : m_LiquidDensity;

		// Above water, don't touch any forces
		if(depth >= (waterHeight + rfc.MaxDepth)) {
			return;
		}

		Vec3 force = Vec3(0,liquidDensity * rfc.Volume,0);

		if(depth > waterHeight - rfc.MaxDepth)
		{
			auto t = (-depth + rfc.MaxDepth + waterHeight) / (rfc.MaxDepth * 2.0);
			force.y *= t;
		}

		pc.AddForceAtPoint(force, worldPoint);

	}
} // namespace Voxymore::Core
