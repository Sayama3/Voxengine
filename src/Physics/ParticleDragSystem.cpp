//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/ParticleDragSystem.hpp"
#include "Voxymore/Physics/ParticleComponent.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"


namespace Voxymore::Core
{
	VXM_CREATE_SYSTEM(ParticleDragSystem);

	void ParticleDragSystem::DeserializeSystem(YAML::Node &node)
	{
		System::DeserializeSystem(node);
		auto DragCoefNode = node["DragCoef"];
		if(DragCoefNode.IsDefined())
		{
			m_DragCoef = DragCoefNode.as<Real>();
		}
		else
		{
			m_DragCoef = 0.9;
		}

		auto DragCoefSqrNode = node["DragCoefSqr"];
		if(DragCoefSqrNode.IsDefined())
		{
			m_DragCoefSqr = DragCoefSqrNode.as<Real>();
		}
		else
		{
			m_DragCoefSqr = 0.8;
		}
	}

	void ParticleDragSystem::SerializeSystem(YAML::Emitter & out)
	{
		System::SerializeSystem(out);
		out << KEYVAL("DragCoef", m_DragCoef);
		out << KEYVAL("DragCoefSqr", m_DragCoefSqr);
	}

	void ParticleDragSystem::ResetSystem()
	{
		m_DragCoef = 0.9;
		m_DragCoefSqr = 0.8;
	}

	bool ParticleDragSystem::OnImGuiRender()
	{
		bool changed = System::OnImGuiRender();
		changed |= ImGuiLib::DragReal("DragCoef", &m_DragCoef);
		changed |= ImGuiLib::DragReal("DragCoefSqr", &m_DragCoefSqr);
		return changed;
	}

	void ParticleDragSystem::Update(Scene &scene, TimeStep ts)
	{
		auto& reg = scene.GetRegistry();
		auto view = reg.view<ParticleComponent>(entt::exclude<DisableComponent>);
		for(auto e : view)
		{
			auto& pc = view.get<ParticleComponent>(e);
			Vec3 force = pc.GetVelocity();
			Real drag = Math::SqrMagnitude(force);
			if(drag == 0) continue;
			drag = Math::Sqrt(drag);
			drag = m_DragCoef * drag + m_DragCoefSqr * drag * drag;
			Math::Normalize(force);
			force *= -drag;
			pc.AddForce(force);
		}
	}
} // namespace Voxymore::Core

