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
		m_Material = node["Material"].as<MaterialField>();
		m_Degree = node["Degree"].as<int>(1);
		m_Definition = node["Definition"].as<int>(1000);

		m_Points.clear();
		auto pointsNode = node["Points"];
		if(pointsNode && pointsNode.IsSequence())
		{
			m_Points.reserve(pointsNode.size());
			for(auto pointNode : pointsNode)
			{
				m_Points.push_back(pointNode.as<Vec3>(Vec3{0,0,0}));
			}
		}

		m_Nodes.clear();
		auto nodesNode = node["Nodes"];
		if(nodesNode && nodesNode.IsSequence())
		{
			m_Nodes.reserve(nodesNode.size());
			for(auto nodeNode : nodesNode)
			{
				m_Nodes.push_back(nodeNode.as<float>(-1));
			}
		}

		m_Weight.clear();
		auto weightsNode = node["Weights"];
		if(weightsNode && weightsNode.IsSequence())
		{
			m_Weight.reserve(weightsNode.size());
			for(auto weightNode : weightsNode)
			{
				m_Weight.push_back(weightNode.as<float>(1));
			}
		}

		m_Matrices.clear();
		auto matricesNode = node["Matrices"];
		if(matricesNode && matricesNode.IsSequence())
		{
			m_Matrices.reserve(matricesNode.size());
			for(auto matriceNode : matricesNode)
			{
				m_Matrices.push_back(matriceNode.as<Mat4>(Math::Identity<glm::mat4>()));
			}
		}
	}

	void BSplinesComponents::SerializeComponent(YAML::Emitter &out, Entity e)
	{
		out << KEYVAL("Material", m_Material);
		out << KEYVAL("Degree", m_Degree);
		out << KEYVAL("Definition", m_Definition);
		out << KEYVAL("Points", YAML::BeginSeq);
		{
			for (const auto& p : m_Points) {
				out << p;
			}
			out << YAML::EndSeq;
		}
		out << KEYVAL("Nodes", YAML::BeginSeq);
		{
			for (const auto& p : m_Nodes) {
				out << p;
			}
			out << YAML::EndSeq;
		}
		out << KEYVAL("Weights", YAML::BeginSeq);
		{
			for (const auto& p : m_Weight) {
				out << p;
			}
			out << YAML::EndSeq;
		}
		out << KEYVAL("Matrices", YAML::BeginSeq);
		{
			for (const auto& m : m_Matrices) {
				out << m;
			}
			out << YAML::EndSeq;
		}
	}


	bool DragFloat(const char* label, float* f)
	{
		return ImGui::DragFloat(label, f, 0.05);
	}

	bool BSplinesComponents::OnImGuiRender(Entity e)
	{
		bool changed = false;


		// Matrices ImGui
		{
			const ImGuiTreeNodeFlags treeNodeFlags =
					ImGuiTreeNodeFlags_DefaultOpen |
					ImGuiTreeNodeFlags_AllowItemOverlap |
					ImGuiTreeNodeFlags_SpanAvailWidth |
					ImGuiTreeNodeFlags_FramePadding |
					ImGuiTreeNodeFlags_Framed;
			ImVec2 contentAvailable = ImGui::GetContentRegionMax();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			float buttonWidth = lineHeight * 4;
			bool open = ImGui::TreeNodeEx((void *) typeid(glm::mat4).hash_code(), treeNodeFlags, "%s", "Matrices");
			ImGui::PopStyleVar();

			ImGui::SameLine(contentAvailable.x - buttonWidth + 4);
			if (ImGui::Button("+", ImVec2(buttonWidth, lineHeight))) {
				m_Matrices.push_back(Math::Identity<glm::mat4>());
				changed = true;
			}
			if (open) {
				for (int i = 0; i < m_Matrices.size(); ++i) {
					ImGui::PushID(i);
					ImGui::Separator();
					ImGui::Text("Matrix %d", i);
					ImGui::SameLine();
					if (ImGui::Button("Reset")) {
						m_Matrices[i] = Math::Identity<Mat4>();
						changed = true;
					}
					changed |= ImGui::DragFloat4("c0", glm::value_ptr(m_Matrices[i][0]));
					changed |= ImGui::DragFloat4("c1", glm::value_ptr(m_Matrices[i][1]));
					changed |= ImGui::DragFloat4("c2", glm::value_ptr(m_Matrices[i][2]));
					changed |= ImGui::DragFloat4("c3", glm::value_ptr(m_Matrices[i][3]));
					ImGui::PushStyleColor(ImGuiCol_Button, ImGuiLib::c_RedColor);
					if (ImGui::Button("Delete")) {
						m_Matrices.erase(m_Matrices.begin() + i);
						changed = true;
						--i;
					}
					ImGui::PopStyleColor();
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}

		changed |= ImGuiLib::DrawAssetField("Material", &m_Material);
		changed |= ImGui::DragInt("Degree", &m_Degree, 1, 1, INT_MAX);
		changed |= ImGui::DragInt("Definition", &m_Definition, 1, 1, INT_MAX);
		changed |= ImGuiLib::DrawVector<glm::vec3>("Points", &m_Points, [](const char* c, glm::vec3* p)->bool{return ImGui::DragFloat3(c,glm::value_ptr(*p));}, glm::vec3(0), m_Degree, UINT64_MAX);
		changed |= ImGuiLib::DrawVector<float>("Weights", &m_Weight, VXM_BIND_FN(DragFloat), 1.0f, m_Points.size(), m_Points.size());
		changed |= ImGuiLib::DrawVector<float>("Nodes", &m_Nodes, VXM_BIND_FN(DragFloat));

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

	std::vector<glm::vec3> BSplinesComponents::GetWorldPoints(const Mat4& transform) const
	{
		VXM_PROFILE_FUNCTION();
		std::vector<Vec3> result(m_Points.size());
		auto matrix = Math::Identity<glm::mat4>();
		for(const auto& mat : m_Matrices) matrix = mat * matrix;
		matrix = transform * matrix;
		std::transform(m_Points.begin(), m_Points.end(), result.begin(), [&matrix](const Vec3& p) {return Math::TransformPoint(matrix, p);});
		return result;
	}
} // namespace Voxymore::Core
