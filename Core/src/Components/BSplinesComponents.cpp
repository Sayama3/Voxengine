//
// Created by ianpo on 13/04/2024.
//

#include "Voxymore/Components/BSplinesComponents.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include <ImGuizmo.h>


namespace Voxymore::Core
{

	void BSplinesComponents::DeserializeComponent(YAML::Node &node, Entity e)
	{
	}

	void BSplinesComponents::SerializeComponent(YAML::Emitter &out, Entity e)
	{
	}

	bool BSplinesComponents::OnImGuiRender(Entity e)
	{
		bool changed = false;

		changed |= ImGuiLib::DrawAssetField("Material", &m_Material);
		changed |= ImGui::DragInt("Degree", &m_Degree, 1, 1, INT_MAX);
		changed |= ImGuiLib::DrawVector<glm::vec3>("Points", &m_Points, [](const char* c, glm::vec3* p)->bool{return ImGui::DragFloat3(c,glm::value_ptr(*p));}, glm::vec3(0), m_Degree+1, UINT64_MAX);
		changed |= ImGuiLib::DrawVector<float>("Weights", &m_Weight, VXM_BIND_FN(ImGui::DragFloat), 0.0f, m_Points.size(), m_Points.size());
		changed |= ImGuiLib::DrawVector<int>("Nodes", &m_Nodes, VXM_BIND_FN(ImGui::DragInt), 0, m_Points.size() - m_Degree, m_Points.size() - m_Degree);

//		int k = m_Points.size();
//		int m = m_Degree;

		return changed;
	}

	bool BSplinesComponents::OnImGuizmo(Entity e, const float *viewMatrix, const float *projectionMatrix)
	{
		bool changed = false;
		glm::mat4 localToWorld = e.GetComponent<TransformComponent>().GetTransform();
		glm::mat4 worldToLocal = Math::Inverse(localToWorld);

		glm::mat4 point;
		for (int i = 0; i < m_Points.size(); ++i) {
			point = Math::Identity<glm::mat4>();
			auto worldPoint = localToWorld * glm::vec4(m_Points[i],1);
			point[3] = worldPoint;
			ImGuizmo::PushID(i);
			ImGuizmo::Manipulate(viewMatrix, projectionMatrix, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD, glm::value_ptr(point), nullptr, nullptr);
			if (ImGuizmo::IsUsing()) {
				glm::vec3 localPoint = worldToLocal * point[3];
				m_Points[i] = localPoint;
				changed = true;
			}
			ImGuizmo::PopID();
		}
		return changed;
	}

	bool BSplinesComponents::IsValid() const
	{
		return m_Degree >= 1 &&
			   m_Points.size() >= m_Degree + 1 &&
			   m_Weight.size() == m_Points.size() &&
			   m_Nodes.size() == m_Points.size() - m_Degree &&
			   m_Nodes.size() == m_Points.size() - m_Degree;
	}
} // namespace Voxymore::Core
