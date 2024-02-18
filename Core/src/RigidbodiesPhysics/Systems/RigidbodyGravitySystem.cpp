//
// Created by Iannis on 12/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Systems/RigidbodyGravitySystem.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

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
	VXM_DECLARE_SYSTEM(RigidbodyGravitySystem);

	void RigidbodyGravitySystem::DeserializeSystem(YAML::Node &node)
	{
		System::DeserializeSystem(node);
		DeserializeField(node, m_Gravity, "Gravity", Vec3, Math::Gravity);
	}

	void RigidbodyGravitySystem::SerializeSystem(YAML::Emitter &out)
	{
		System::SerializeSystem(out);
		out << KEYVAL("Gravity", m_Gravity);
	}

	void RigidbodyGravitySystem::ResetSystem()
	{
		m_Gravity = Math::Gravity;
	}

	bool RigidbodyGravitySystem::OnImGuiRender()
	{
		bool changed = false;
		changed |= ImGuiLib::DrawVec3Control("Gravity", m_Gravity);
		return changed;
	}

	void RigidbodyGravitySystem::Update(Scene &scene, TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		auto view = scene.view<RigidbodyComponent>();
		auto func= VXM_BIND_EVENT_FN(UpdateParticle);
		scene.each<RigidbodyComponent>(exclude<DisableComponent>, MultiThreading::ExecutionPolicy::Parallel_Unsequenced, func);

	}

	void RigidbodyGravitySystem::UpdateParticle(entt::entity e, RigidbodyComponent& pc)
	{
		VXM_PROFILE_FUNCTION();
		if (pc.HasFiniteMass()) pc.AddAcceleration(m_Gravity);
	}
} // namespace Voxymore::Core
