//
// Created by Iannis on 12/02/2024.
//

#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

#define DeserializeField(node, fieldVariable, fieldName, type, defaultValue) fieldVariable = node[fieldName].as<type>(defaultValue);

namespace Voxymore::Core
{
	RigidbodyComponent::RigidbodyComponent(Real inverseMass, Real linearDamping, TransformComponent* transform, Mat3 inverseInertiaTensor) : Rigidbody(inverseMass, linearDamping, transform, inverseInertiaTensor)
	{
	}

	void RigidbodyComponent::DeserializeComponent(YAML::Node &node, Entity entity)
	{
		VXM_PROFILE_FUNCTION();
		DeserializeField(node, m_InverseMass, "InverseMass", Real, 1);
		DeserializeField(node, m_LinearDamping, "LinearDamping", Real, 0.9);
		DeserializeField(node, m_AngularDamping, "AngularDamping", Real, 0.9);
		DeserializeField(node, m_LinearVelocity, "LinearVelocity", Vec3, Vec3(0));
		DeserializeField(node, m_AngularVelocity, "AngularVelocity", Vec3, Vec3(0));
		DeserializeField(node, m_InverseInertiaTensor, "InverseInertiaTensor", Mat3, Math::Identity<Mat3>());
		DeserializeField(node, m_ForceAccumulate, "ForceAccumulate", Vec3, Vec3(0));
		DeserializeField(node, m_TorqueAccumulate, "TorqueAccumulate", Vec3, Vec3(0));
		DeserializeField(node, m_Acceleration, "Acceleration", Vec3, Math::Gravity);
//		DeserializeField(node, m_IsAwake, "IsAwake", bool, false);

		if(node["Disable"].as<bool>(false) && !entity.HasComponent<DisableRigidbody>())
		{
			entity.AddEmptyComponent<DisableRigidbody>();
		}

		if (!m_Transform)
		{
			m_Transform = &entity.GetComponent<TransformComponent>();
		}
	}

	void RigidbodyComponent::SerializeComponent(YAML::Emitter &out, Entity entity)
	{
		VXM_PROFILE_FUNCTION();
		out << KEYVAL("InverseMass", m_InverseMass);
		out << KEYVAL("LinearDamping", m_LinearDamping);
		out << KEYVAL("AngularDamping", m_AngularDamping);
		out << KEYVAL("LinearVelocity", m_LinearVelocity);
		out << KEYVAL("AngularVelocity", m_AngularVelocity);
		out << KEYVAL("InverseInertiaTensor", m_InverseInertiaTensor);
		out << KEYVAL("ForceAccumulate", m_ForceAccumulate);
		out << KEYVAL("TorqueAccumulate", m_TorqueAccumulate);
		out << KEYVAL("Acceleration", m_Acceleration);
//		out << KEYVAL("IsAwake", m_IsAwake);
		out << KEYVAL("Disable", entity.HasComponent<DisableRigidbody>());
	}

	bool RigidbodyComponent::OnImGuiRender(Entity entity)
	{
		VXM_PROFILE_FUNCTION();

		if (!m_Transform)
		{
			m_Transform = &entity.GetOrAddComponent<TransformComponent>();
		}

		bool changed = false;

		bool IsDisable = entity.HasComponent<DisableRigidbody>();

		if(ImGui::Checkbox("Disable Physics", &IsDisable))
		{
			changed = true;
			if(IsDisable)
			{
				entity.AddEmptyComponent<DisableRigidbody>();
			}
			else
			{
				entity.RemoveComponent<DisableRigidbody>();
			}
		}

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
		if(ImGuiLib::DragReal("Mass", &mass, 1, 0, REAL_MAX))
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
		changed |= ImGuiLib::DragReal3("Linear Velocity", Math::ValuePtr(m_LinearVelocity));
		changed |= ImGuiLib::DragReal3("Angular Velocity", Math::ValuePtr(m_AngularVelocity));
		ImGui::Spacing();

		auto tensor = glm::inverse(m_InverseInertiaTensor);
		auto tensorChanged = false;
		ImGui::LabelText("Inertia Tensor","");
		tensorChanged |= ImGuiLib::DragReal3("##Inertia Tensor_c1", Math::ValuePtr(tensor[0]), 0.01, 0,0,"%.2f");
		tensorChanged |= ImGuiLib::DragReal3("##Inertia Tensor_c2", Math::ValuePtr(tensor[1]), 0.01, 0,0,"%.2f");
		tensorChanged |= ImGuiLib::DragReal3("##Inertia Tensor_c3", Math::ValuePtr(tensor[2]), 0.01, 0,0,"%.2f");
		if(tensorChanged)
		{
			changed = true;
			m_InverseInertiaTensor = glm::inverse(tensor);
		}

		ImGui::Spacing();
		changed |= ImGuiLib::DragReal3("Force Accumulate", Math::ValuePtr(m_ForceAccumulate));
		changed |= ImGuiLib::DragReal3("Torque Accumulate", Math::ValuePtr(m_TorqueAccumulate));
		ImGui::Spacing();
		changed |= ImGuiLib::DragReal3("Acceleration", Math::ValuePtr(m_Acceleration));
		ImGui::Spacing();
//		changed |= ImGui::Checkbox("IsAwake", &m_IsAwake);

		if(ImGui::CollapsingHeader("Inertia Tensors"))
		{
			ImGui::BeginDisabled(!HasFiniteMass());

			enum ShapeType { SHAPE_CUBE, SHAPE_FULL_SPHERE, SHAPE_EMPTY_SPHERE, SHAPE_ELLIPSOID, SHAPE_CYLINDER, SHAPE_CONE, SHAPE_HEMISPHERE};
			const char* items[] = { "Cube", "Full Sphere", "Empty Sphere", "Ellipsoid", "Cylinder", "Cone", "Hemisphere" };
			static int current_shape = SHAPE_CUBE;

			ImGui::Combo("Shape Type", &current_shape, items, IM_ARRAYSIZE(items));

			static Real radius = 1;
			static Vec3 radius3d = {1,1,1};
			static Vec3 size = {1,1,1};
			static Real height = 1;

			switch((ShapeType)current_shape)
				{
					case SHAPE_CUBE: {
						ImGuiLib::DragReal3("Cube Size", &size[0]);
						if(ImGui::Button("Calculate Cube Tensor"))
						{
							tensor = Math::Identity<Mat3>();
							auto constant = (1.f/12.f) * mass;

							tensor[0][0] = constant * (size.y * size.y + size.z * size.z);
							tensor[1][1] = constant * (size.x * size.x + size.z * size.z);
							tensor[2][2] = constant * (size.x * size.x + size.y * size.y);

							changed = true;
							m_InverseInertiaTensor = glm::inverse(tensor);
						}
						break;
					}
					case SHAPE_FULL_SPHERE: {
						ImGuiLib::DragReal("Sphere Radius", &radius);
						if(ImGui::Button("Calculate Full Sphere Tensor"))
						{
							auto rad = glm::radians(radius);
							tensor = Math::Identity<Mat3>();
							auto constant = (2.f/5.f) * mass * (rad * rad);
							tensor[0][0] = constant;
							tensor[1][1] = constant;
							tensor[2][2] = constant;

							changed = true;
							m_InverseInertiaTensor = glm::inverse(tensor);
						}
						break;
					}
					case SHAPE_EMPTY_SPHERE: {
						ImGuiLib::DragReal("Sphere Radius", &radius);
						if(ImGui::Button("Calculate Empty Sphere Tensor"))
						{
							auto rad = glm::radians(radius);
							tensor = Math::Identity<Mat3>();
							auto constant = (2.f/3.f) * mass * (rad * rad);
							tensor[0][0] = constant;
							tensor[1][1] = constant;
							tensor[2][2] = constant;

							changed = true;
							m_InverseInertiaTensor = glm::inverse(tensor);
						}
						break;
					}
					case SHAPE_ELLIPSOID: {
						ImGuiLib::DragReal3("Ellipsoid Radius", &radius3d[0]);
						if(ImGui::Button("Calculate Ellipsoid Tensor"))
						{
							auto rad = glm::radians(radius3d);
							tensor = Math::Identity<Mat3>();
							auto constant = (1.f/5.f) * mass;
							tensor[0][0] = constant * (rad.y * rad.y + rad.z * rad.z);
							tensor[1][1] = constant * (rad.x * rad.x + rad.z * rad.z);
							tensor[2][2] = constant * (rad.x * rad.x + rad.y * rad.y);

							changed = true;
							m_InverseInertiaTensor = glm::inverse(tensor);
						}
						break;
					}
					case SHAPE_CYLINDER: {
						ImGuiLib::DragReal("Cylinder Height", &height);
						ImGuiLib::DragReal("Cylinder Radius", &radius);
						if(ImGui::Button("Calculate Cylinder Tensor"))
						{
							auto rad = glm::radians(radius3d);
							tensor = Math::Identity<Mat3>();
							const Real constantC = (Real)(1./12.) * mass * height * height;
							const Real constantS = (Real)(1./4.) * mass * radius * radius;
							tensor[0][0] = constantC + constantS;
							tensor[1][1] = (0.5f) * mass;
							tensor[2][2] = constantC + constantS;

							changed = true;
							m_InverseInertiaTensor = glm::inverse(tensor);
						}
						break;
					}
					//TODO Add Emtpy Cylinder (hollow so with inner and outer radius);
					case SHAPE_CONE: {
						ImGuiLib::DragReal("Cone Height", &height);
						ImGuiLib::DragReal("Cone Radius", &radius);
						if(ImGui::Button("Calculate Cone Tensor"))
						{
							auto rad = glm::radians(radius3d);
							tensor = Math::Identity<Mat3>();
							const Real height2 = height * height;
							const Real radius2 = radius * radius;
							tensor[0][0] = ((Real)(3./80.) * mass * height2) + ((Real)(3./20.) * mass * radius2);
							tensor[1][1] = (Real)(3./10.) * mass * height2;
							tensor[2][2] = ((Real)(3./5.) * mass * height2) + ((Real)(3./20.) * mass * radius2);

							changed = true;
							m_InverseInertiaTensor = glm::inverse(tensor);
						}
						break;
					}
					case SHAPE_HEMISPHERE: {
						ImGuiLib::DragReal("Hemisphere Radius", &radius);
						if(ImGui::Button("Calculate Hemisphere Tensor"))
						{
							auto rad = glm::radians(radius);
							tensor = Math::Identity<Mat4>();
							const Real c = mass * rad * rad;
							tensor[0][0] = (Real)(83./320.) * c;
							tensor[1][1] = (Real)(2./5.) * c;
							tensor[2][2] = (Real)(83./320.) * c;
						}
						break;
					}
				}


			ImGui::EndDisabled();

		}
		return changed;
	}
} // namespace Voxymore::Core
