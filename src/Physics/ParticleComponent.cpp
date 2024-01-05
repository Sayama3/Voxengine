//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/ParticleComponent.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"

#define DeserializeField(COMP, node, fieldVariable, fieldName, type, defaultValue)														\
auto VXM_COMBINE(fieldVariable, Node) = node[fieldName]; 																			\
if(VXM_COMBINE(fieldVariable, Node).IsDefined())																						\
{																																		\
COMP.fieldVariable = VXM_COMBINE(fieldVariable, Node).as<type>();																		\
}																																		\
else {																																	\
	COMP.fieldVariable = defaultValue;																									\
		VXM_CORE_WARNING("We didn't found the field '{0}'. Initializing it at {1}", #fieldName, Math::to_string(COMP.fieldVariable));	\
}


namespace Voxymore::Core
{
	VXM_CREATE_LIGHT_COMPONENT(ParticleComponent)

	ParticleComponent::ParticleComponent() : Particle()
	{}
	ParticleComponent::~ParticleComponent() = default;

	ParticleComponent::ParticleComponent(Vec3 acceleration, Vec3 velocity, Real damping, Real mass) : Particle(Vec3(0), velocity, acceleration, damping, (Real)1.0/mass)
	{}

	void ParticleComponent::DeserializeComponent(YAML::Node &node, ::Voxymore::Core::Entity targetEntity)
	{
		VXM_PROFILE_FUNCTION();

		auto& pc = targetEntity.GetOrAddComponent<ParticleComponent>();

		DeserializeField(pc, node, m_Position, "Position", Vec3, Vec3(0.0));
		DeserializeField(pc, node, m_Velocity, "Velocity", Vec3, Vec3(0.0));
		DeserializeField(pc, node, m_Acceleration, "Acceleration", Vec3, Vec3(0.0));
		DeserializeField(pc, node, m_Damping, "Damping", Real, 0.9);
		DeserializeField(pc, node, m_InverseMass, "InverseMass", Real, 1.0);
	}

	void ParticleComponent::SerializeComponent(YAML::Emitter& out, ::Voxymore::Core::Entity sourceEntity)
	{
		VXM_PROFILE_FUNCTION();
		auto& pc = sourceEntity.GetComponent<ParticleComponent>();
		out << KEYVAL("Position", pc.m_Position);
		out << KEYVAL("Acceleration", pc.m_Acceleration);
		out << KEYVAL("Velocity", pc.m_Velocity);
		out << KEYVAL("Damping", pc.m_Damping);
		out << KEYVAL("InverseMass", pc.m_InverseMass);
	}

	bool ParticleComponent::OnImGuiRender(::Voxymore::Core::Entity sourceEntity)
	{
		VXM_PROFILE_FUNCTION();
		auto& pc = sourceEntity.GetComponent<ParticleComponent>();
		bool changed = false;
		changed |= ImGuiLib::DrawVec3Control("Velocity", pc.m_Velocity);
		changed |= ImGuiLib::DrawVec3Control("Acceleration", pc.m_Acceleration);
		changed |= ImGuiLib::DragReal("Damping", &pc.m_Damping, 0.001, 0, 1);
		Real mass =  1.0 / pc.m_InverseMass;
		changed |= ImGuiLib::DragReal("Mass", &mass, 0.01, 0.01);
		if(changed && mass > 0)
		{
			pc.m_InverseMass = 1.0 / mass;
		}
		return changed;
	}

}// namespace Voxymore::Core