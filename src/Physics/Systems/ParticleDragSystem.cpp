//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/Systems/ParticleDragSystem.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"
#include "Voxymore/Physics/Components/ParticleComponent.hpp"


namespace Voxymore::Core
{
	VXM_CREATE_SYSTEM(ParticleDragSystem);

	void ParticleDragSystem::DeserializeSystem(YAML::Node &node)
	{
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
		System::SerializeSystem(out);
		out << KEYVAL("DragCoef", m_DragCoef);
		out << KEYVAL("DragCoefSqr", m_DragCoefSqr);
	}

	void ParticleDragSystem::ResetSystem()
	{
		VXM_PROFILE_FUNCTION();
		m_DragCoef = 0.9;
		m_DragCoefSqr = 0.8;
	}

	bool ParticleDragSystem::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = System::OnImGuiRender();
		changed |= ImGuiLib::DragReal("DragCoef", &m_DragCoef);
		changed |= ImGuiLib::DragReal("DragCoefSqr", &m_DragCoefSqr);
		return changed;
	}

	void ParticleDragSystem::Update(Scene &scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		scene.each<ParticleComponent>(exclude<DisableComponent>,
		        [=, this](auto entity, ParticleComponent& pc)
			    {
				  VXM_PROFILE_FUNCTION();
				    Vec3 force = pc.GetVelocity();
				    Real drag = Math::SqrMagnitude(force);
				    if(drag == 0) return;
				    drag = Math::Sqrt(drag);
				    drag = m_DragCoef * drag + m_DragCoefSqr * drag * drag;
					force = Math::Normalize(force);
				    force *= -drag;
				    pc.AddForce(force);
			    });
	}
} // namespace Voxymore::Core

