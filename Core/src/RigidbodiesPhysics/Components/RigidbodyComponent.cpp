//
// Created by Iannis on 12/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
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
	RigidbodyComponent::RigidbodyComponent(Real inverseMass, Real linearDamping, Vec3 position, Quat orientation, Mat3 inverseInertiaTensor)
		: Rigidbody(inverseMass, linearDamping, position, orientation, inverseInertiaTensor)
	{
	}

	void RigidbodyComponent::DeserializeComponent(YAML::Node &node)
	{
		VXM_PROFILE_FUNCTION();
		DeserializeField(node, m_InverseMass, "InverseMass", Real, 1);
		DeserializeField(node, m_LinearDamping, "LinearDamping", Real, 0.9);
		DeserializeField(node, m_AngularDamping, "AngularDamping", Real, 0.9);
		DeserializeField(node, m_Position, "Position", Vec3, Vec3(0));
		DeserializeField(node, m_Orientation, "Orientation", Quat, Math::Identity<Quat>());
		DeserializeField(node, m_LinearVelocity, "LinearVelocity", Vec3, Vec3(0));
		DeserializeField(node, m_AngularVelocity, "AngularVelocity", Vec3, Vec3(0));
		DeserializeField(node, m_InverseInertiaTensor, "InverseInertiaTensor", Mat3, Math::Identity<Mat3>());
		DeserializeField(node, m_ForceAccumulate, "ForceAccumulate", Vec3, Vec3(0));
		DeserializeField(node, m_TorqueAccumulate, "TorqueAccumulate", Vec3, Vec3(0));
		DeserializeField(node, m_Acceleration, "Acceleration", Vec3, Math::Gravity);
		DeserializeField(node, m_IsAwake, "IsAwake", bool, false);
	}

	void RigidbodyComponent::SerializeComponent(YAML::Emitter &out)
	{
		VXM_PROFILE_FUNCTION();
		out << KEYVAL("InverseMass", m_InverseMass);
		out << KEYVAL("LinearDamping", m_LinearDamping);
		out << KEYVAL("AngularDamping", m_AngularDamping);
		out << KEYVAL("Position", m_Position);
		out << KEYVAL("Orientation", m_Orientation);
		out << KEYVAL("LinearVelocity", m_LinearVelocity);
		out << KEYVAL("AngularVelocity", m_AngularVelocity);
		out << KEYVAL("InverseInertiaTensor", m_InverseInertiaTensor);
		out << KEYVAL("ForceAccumulate", m_ForceAccumulate);
		out << KEYVAL("TorqueAccumulate", m_TorqueAccumulate);
		out << KEYVAL("Acceleration", m_Acceleration);
		out << KEYVAL("IsAwake", m_IsAwake);
	}

	bool RigidbodyComponent::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;

		// Information about the object.
		if(HasFiniteMass())
		{
			ImGui::TextColored({.2,.8,.3, 1}, "The object is movable.");
		}
		else
		{
			ImGui::TextColored({.8,.2,.3, 1}, "The object is immovable.");
		}

		Real mass = HasFiniteMass() ? GetMass() : 0;
		if(ImGuiLib::DragReal("Mass", &mass, 0, REAL_MAX))
		{
			changed = true;
			if(mass <= glm::epsilon<Real>())
			{
				SetInverseMass(0);
			}
			else
			{
				SetMass(mass);
			}
		}


		changed |= ImGuiLib::DragReal("Linear Damping", &m_LinearDamping, 0.001, 0, 1, "%.3f");
		changed |= ImGuiLib::DragReal("Angular Damping", &m_AngularDamping, 0.001, 0, 1, "%.3f");
		ImGui::Spacing();
		ImGui::BeginDisabled(true);
		changed |= ImGuiLib::DrawVec3Control("Position", m_Position);
		changed |= ImGuiLib::DragReal4("Orientation", glm::value_ptr(m_Orientation));
		ImGui::Spacing();
		ImGui::EndDisabled();
		changed |= ImGuiLib::DragReal3("Linear Velocity", glm::value_ptr(m_LinearVelocity));
		changed |= ImGuiLib::DragReal3("Angular Velocity", glm::value_ptr(m_AngularVelocity));
		ImGui::Spacing();

		auto tensor = glm::inverse(m_InverseInertiaTensor);
		auto tensorChanged = false;
		ImGui::LabelText("Inertia Tensor","");
		tensorChanged |= ImGuiLib::DragReal3("##Inertia Tensor_c1", glm::value_ptr(tensor[0]), 0.01, 0,0,"%.2f");
		tensorChanged |= ImGuiLib::DragReal3("##Inertia Tensor_c2", glm::value_ptr(tensor[1]), 0.01, 0,0,"%.2f");
		tensorChanged |= ImGuiLib::DragReal3("##Inertia Tensor_c3", glm::value_ptr(tensor[2]), 0.01, 0,0,"%.2f");
		if(tensorChanged)
		{
			changed = true;
			m_InverseInertiaTensor = glm::inverse(tensor);
		}

		ImGui::Spacing();
		changed |= ImGuiLib::DragReal3("Force Accumulate", glm::value_ptr(m_ForceAccumulate));
		changed |= ImGuiLib::DragReal3("Torque Accumulate", glm::value_ptr(m_TorqueAccumulate));
		ImGui::Spacing();
		changed |= ImGuiLib::DragReal3("Acceleration", glm::value_ptr(m_Acceleration));
		ImGui::Spacing();
		changed |= ImGui::Checkbox("IsAwake", &m_IsAwake);
		return changed;
	}
} // namespace Voxymore::Core
