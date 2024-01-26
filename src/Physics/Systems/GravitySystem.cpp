//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/Systems/GravitySystem.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"
#include "Voxymore/Physics/Components/ParticleComponent.hpp"


namespace Voxymore::Core
{
	VXM_DECLARE_SYSTEM(GravitySystem);

	void GravitySystem::DeserializeSystem(YAML::Node& node)
	{
		VXM_PROFILE_FUNCTION();
		System::DeserializeSystem(node);

		auto gravNode = node["Gravity"];
		if(gravNode.IsDefined())
		{
			m_Gravity = gravNode.as<Vec3>();
		}
		else
		{
			m_Gravity = Math::Gravity;
		}
	}

	void GravitySystem::SerializeSystem(YAML::Emitter& out)
	{
		VXM_PROFILE_FUNCTION();
		System::SerializeSystem(out);

		out << KEYVAL("Gravity", m_Gravity);
	}

	void GravitySystem::ResetSystem()
	{
		VXM_PROFILE_FUNCTION();
		m_Gravity = Math::Gravity;
	}

	bool GravitySystem::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = System::OnImGuiRender();

		changed |= ImGuiLib::DrawVec3Control("Gravity", m_Gravity);

		return changed;
	}

	void GravitySystem::Update(Scene &scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		auto func = VXM_BIND_EVENT_FN(UpdateParticle);
		scene.each<ParticleComponent>(exclude<DisableComponent>, MultiThreading::ExecutionPolicy::Parallel_Unsequenced, func);
	}

	void GravitySystem::UpdateParticle(entt::entity e, ParticleComponent& pc)
	{
		VXM_PROFILE_FUNCTION();
		pc.AccumulateAcceleration(m_Gravity);
	}

} // namespace Voxymore::Core
