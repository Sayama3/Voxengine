//
// Created by ianpo on 05/01/2024.
//

#include "Voxymore/Physics/ParticleComponent.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGUI/ImGuiLib.hpp"

#define DeserializeField(COMP, node, fieldName, type, defaultValue)																	\
auto VXM_COMBINE(fieldName, Node) = node[#fieldName]; 																				\
if(VXM_COMBINE(fieldName, Node).IsDefined())																						\
{																																	\
	COMP.fieldName = VXM_COMBINE(fieldName, Node).as<type>();																		\
}																																	\
else {																																\
		COMP.fieldName = defaultValue;																								\
		VXM_CORE_WARNING("We didn't found the field '{0}'. Initializing it at {1}", #fieldName, Math::to_string(COMP.fieldName));	\
}


namespace Voxymore::Core
{
	VXM_CREATE_LIGHT_COMPONENT(ParticleComponent)

	void ParticleComponent::DeserializeComponent(YAML::Node &node, ::Voxymore::Core::Entity targetEntity)
	{
		VXM_PROFILE_FUNCTION();

		auto& pc = targetEntity.GetOrAddComponent<ParticleComponent>();

		DeserializeField(pc, node, Velocity, Vec3, Vec3(0.0));
		DeserializeField(pc, node, Acceleration, Vec3, Vec3(0.0));
		DeserializeField(pc, node, Damping, Real, 0.5);
		DeserializeField(pc, node, InverseMass, Real, 1.0);
	}

	ParticleComponent::ParticleComponent(Vec3 acceleration, Vec3 velocity, Real damping, Real mass) : Acceleration(acceleration), Velocity(velocity), Damping(damping), InverseMass(1.0)
	{
		VXM_CORE_ASSERT(mass > 0.0, "The mass cannot be bellow 0.");
		if(mass > 0)
		{
			InverseMass = 1.0 / mass;
		}

	}

	void ParticleComponent::SerializeComponent(YAML::Emitter& out, ::Voxymore::Core::Entity sourceEntity)
	{
		VXM_PROFILE_FUNCTION();
		auto& pc = sourceEntity.GetComponent<ParticleComponent>();
		out << KEYVAL("Acceleration", pc.Acceleration);
		out << KEYVAL("Velocity", pc.Velocity);
		out << KEYVAL("Damping", pc.Damping);
		out << KEYVAL("InverseMass", pc.InverseMass);
	}

	bool ParticleComponent::OnImGuiRender(::Voxymore::Core::Entity sourceEntity)
	{
		VXM_PROFILE_FUNCTION();
		auto& pc = sourceEntity.GetComponent<ParticleComponent>();
		bool changed = false;
		changed |= ImGuiLib::DrawVec3Control("Velocity", pc.Velocity);
		changed |= ImGuiLib::DrawVec3Control("Acceleration", pc.Acceleration);
		changed |= ImGuiLib::DragReal("Damping", &pc.Damping, 0.001, 0, 1);
		Real mass =  1.0 / pc.InverseMass;
		changed |= ImGuiLib::DragReal("Mass", &mass, 0.01, 0.01);
		if(changed && mass > 0)
		{
			pc.InverseMass = 1.0 / mass;
		}
		return changed;
	}

	Particle ParticleComponent::GetParticle(Entity e) const
	{
		VXM_PROFILE_FUNCTION();
		return GetParticle(e.GetComponent<TransformComponent>().GetPosition());
	}
	Particle ParticleComponent::GetParticle(const Vec3 &position) const
	{
		VXM_PROFILE_FUNCTION();
		Particle p(position, Velocity ,Acceleration, Damping);
		return p;
	}

}// namespace Voxymore::Core