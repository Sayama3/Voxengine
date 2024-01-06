//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/Systems/GravitySystem.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"
#include "Voxymore/Physics/Components/ParticleComponent.hpp"


namespace Voxymore::Core
{
	VXM_CREATE_SYSTEM(GravitySystem);

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
			ResetSystem();
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
		m_Gravity = Vec3(0.0, -9.81, 0.0);
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
		scene.each<ParticleComponent>(exclude<DisableComponent>, VXM_BIND_EVENT_FN(UpdateParticle));
	}

	void GravitySystem::UpdateParticle(entt::entity e, ParticleComponent& pc)
	{
		VXM_PROFILE_FUNCTION();
		pc.AddAcceleration(m_Gravity);
	}

} // namespace Voxymore::Core
