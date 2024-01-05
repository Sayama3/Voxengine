//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/GravitySystem.hpp"
#include "Voxymore/Physics/ParticleComponent.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"


namespace Voxymore::Core
{
	VXM_CREATE_SYSTEM(GravitySystem);

	void GravitySystem::DeserializeSystem(YAML::Node& node)
	{
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
		System::SerializeSystem(out);

		out << KEYVAL("Gravity", m_Gravity);
	}

	void GravitySystem::ResetSystem()
	{
		m_Gravity = Vec3(0.0, -9.81, 0.0);
	}

	bool GravitySystem::OnImGuiRender()
	{
		bool changed = System::OnImGuiRender();

		changed |= ImGuiLib::DrawVec3Control("Gravity", m_Gravity);

		return changed;
	}

	void GravitySystem::Update(Scene &scene, TimeStep ts)
	{
		auto& reg = scene.GetRegistry();
		auto view = reg.view<ParticleComponent>(entt::exclude<DisableComponent>);
		for(auto e : view)
		{
			auto& pc = view.get<ParticleComponent>(e);
			pc.AddAcceleration(m_Gravity);
		}
	}
} // namespace Voxymore::Core
