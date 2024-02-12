//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/Particles/Components/ParticleComponent.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"

#define DeserializeField(node, fieldVariable, fieldName, type, defaultValue)														\
auto VXM_COMBINE(fieldVariable, Node) = node[fieldName]; 																			\
if(VXM_COMBINE(fieldVariable, Node).IsDefined())																						\
{																																		\
fieldVariable = VXM_COMBINE(fieldVariable, Node).as<type>();																		\
}																																		\
else {																																	\
	fieldVariable = defaultValue;																									\
		VXM_CORE_WARNING("We didn't found the field '{0}'. Initializing it at {1}", #fieldName, Math::to_string(fieldVariable));	\
}


namespace Voxymore::Core
{
	VXM_CREATE_LIGHT_COMPONENT(ParticleComponent)

	ParticleComponent::ParticleComponent() : Particle()
	{}
	ParticleComponent::~ParticleComponent() = default;

	ParticleComponent::ParticleComponent(Vec3 acceleration, Vec3 velocity, Real damping, Real mass) : Particle(Vec3(0), velocity, acceleration, damping, (Real)1.0/mass)
	{}

	void ParticleComponent::DeserializeComponent(YAML::Node &node)
	{
		VXM_PROFILE_FUNCTION();

		DeserializeField(node, m_Position, "Position", Vec3, Vec3(0.0));
		DeserializeField(node, m_Velocity, "Velocity", Vec3, Vec3(0.0));
		DeserializeField(node, m_Acceleration, "Acceleration", Vec3, Vec3(0.0));
		DeserializeField(node, m_Damping, "Damping", Real, 0.9);
		DeserializeField(node, m_InverseMass, "InverseMass", Real, 1.0);
	}

	void ParticleComponent::SerializeComponent(YAML::Emitter& out)
	{
		VXM_PROFILE_FUNCTION();
		out << KEYVAL("Position", m_Position);
		out << KEYVAL("Acceleration", m_Acceleration);
		out << KEYVAL("Velocity", m_Velocity);
		out << KEYVAL("Damping", m_Damping);
		out << KEYVAL("InverseMass", m_InverseMass);
	}

	bool ParticleComponent::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;

		auto region = ImGui::GetContentRegionAvail();
		changed |= ImGuiLib::DrawVec3Control("Velocity", m_Velocity);
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		max.x += region.x;
		auto mouse = ImGui::GetMousePos();
		if (ImGui::IsMouseHoveringRect(min, max))
		{
			ImGui::SetTooltip("Speed : %f m/s", Math::Magnitude(m_Velocity));
		}

		changed |= ImGuiLib::DrawVec3Control("Acceleration", m_Acceleration);
		changed |= ImGuiLib::DragReal("Damping", &m_Damping, 0.001, 0, 1);
		Real mass =  1.0 / m_InverseMass;
		changed |= ImGuiLib::DragReal("Mass", &mass, 0.01, 0.01);
		if(changed && mass > 0)
		{
			m_InverseMass = 1.0 / mass;
		}
		return changed;
	}

}// namespace Voxymore::Core