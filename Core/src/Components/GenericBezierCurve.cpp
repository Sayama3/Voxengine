//
// Created by Iannis on 02/04/2024.
//

#include "Voxymore/Components/GenericBezierCurve.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include <ImGuizmo.h>

namespace Voxymore::Core
{

	void GenericBezierCurve::DeserializeComponent(YAML::Node& node, Entity e)
	{
		VXM_PROFILE_FUNCTION();
		m_Continuity = node["Continuity"].as<Continuity>(Continuity::Position);
		m_ControlPoints = node["ControlPoints"].as<int>(2);
		Definition = node["Definition"].as<int>(1000);
		ShaderName = node["ShaderName"].as<std::string>("Bezier");

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

	void GenericBezierCurve::SerializeComponent(YAML::Emitter& out, Entity e)
	{
		VXM_PROFILE_FUNCTION();
		out << KEYVAL("Continuity", m_Continuity);
		out << KEYVAL("Definition", Definition);
		out << KEYVAL("ControlPoints", m_ControlPoints);
		out << KEYVAL("ShaderName", ShaderName);
		out << KEYVAL("Points", YAML::BeginSeq);
		{
			for (const auto& p : m_Points) {
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

	bool GenericBezierCurve::OnImGuiRender(Entity e)
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;
		static const char*continuityEnumsStrings[4] = {"None", "Position", "Velocity", "Acceleration"};
		if(ImGui::Combo("Continuity", (int*)&m_Continuity, continuityEnumsStrings, IM_ARRAYSIZE(continuityEnumsStrings)))
		{
			changed = true;
			ContinuityChanged();
		}

		changed |= ImGui::DragInt("Definition", &Definition, 1, 0, INT_MAX);

		if(ImGui::DragInt("Control Points", &m_ControlPoints, 1, 0, 30))
		{
			changed = true;
			ControlPointsChanged();
		}

		if(ImGui::CollapsingHeader("Matrices")) {
			ImGui::PushID("New Matrix");
			{
				static glm::mat4 matrix = Math::Identity<glm::mat4>();
				if(ImGui::Button("Add"))
				{
					changed = true;
					m_Matrices.push_back(matrix);
				}
				ImGui::SameLine();
				ImGui::Text("New Matrix");
				ImGui::SameLine();
				if (ImGui::Button("Reset")) {
					matrix = Math::Identity<glm::mat4>();
				}
				ImGui::DragFloat4("c0", glm::value_ptr(matrix[0]));
				ImGui::DragFloat4("c1", glm::value_ptr(matrix[1]));
				ImGui::DragFloat4("c2", glm::value_ptr(matrix[2]));
				ImGui::DragFloat4("c3", glm::value_ptr(matrix[3]));
				ImGui::PopID();
			}

			for (int i = 0; i < m_Matrices.size(); ++i) {
				ImGui::PushID(i);
				ImGui::Separator();
				ImGui::Text("Matrix %d",i);
				ImGui::SameLine();
				if (ImGui::Button("Reset")) {
					m_Matrices[i] = Math::Identity<Mat4>();
				}
				changed |= ImGui::DragFloat4("c0", glm::value_ptr(m_Matrices[i][0]));
				changed |= ImGui::DragFloat4("c1", glm::value_ptr(m_Matrices[i][1]));
				changed |= ImGui::DragFloat4("c2", glm::value_ptr(m_Matrices[i][2]));
				changed |= ImGui::DragFloat4("c3", glm::value_ptr(m_Matrices[i][3]));
				if(ImGui::Button("Delete")) {
					m_Matrices.erase(m_Matrices.begin() + i);
					changed = true;
					--i;
				}
				ImGui::PopID();
			}
		}

		if(ImGui::CollapsingHeader("Points"))
		{
			ImGui::PushID("Points");
			const int totalControlPoints = GetTotalControlPoints();

			if(ImGui::Button("Add Point")) {
				AddPoints();
			}
			//TODO: Add button to add Points
			ImGui::BeginDisabled(m_Points.empty());
			if(ImGui::Button("Delete First Segment"))
			{
				m_Points.erase(m_Points.begin(), m_Points.begin() + totalControlPoints);
			}
			ImGui::EndDisabled();

			if(!m_Points.empty())
			{
				bool previousPointsChanged = false;
				VXM_CORE_ASSERT(m_Points.size() % totalControlPoints == 0, "The number of control points is not divisible by {0}", totalControlPoints);

				// First Points
				ImGui::PushID("First Control Segment");
				ImGui::Text("First Control Segment");
				for (int pointIndex = 0; pointIndex < totalControlPoints; ++pointIndex) {
					previousPointsChanged |= ImGuiLib::DrawVec3Control(fmt::format("Point {0}", pointIndex), m_Points[pointIndex]);
				}
				ImGui::PopID();

				// The rest of the points
				for (int controlPoints = totalControlPoints; controlPoints < m_Points.size(); controlPoints += totalControlPoints)
				{
					ImGui::PushID(controlPoints);
					ImGui::Separator();
					ImGui::Text("Control Segment %d", controlPoints/totalControlPoints);
					ImGui::BeginDisabled(true);
					for (int pointIndex = 0; pointIndex < totalControlPoints; ++pointIndex) {
						if(pointIndex == (int)m_Continuity) ImGui::EndDisabled();
						const int index = controlPoints + pointIndex;
						Vec3& point = m_Points[index];
						previousPointsChanged |= ImGuiLib::DrawVec3Control(fmt::format("Point {0}", pointIndex), point);
					}
					if((int)m_Continuity >= totalControlPoints) ImGui::EndDisabled();
					if(ImGui::Button("Delete Segment"))
					{
						m_Points.erase(m_Points.begin() + controlPoints, m_Points.begin() + controlPoints + totalControlPoints);
						controlPoints -= totalControlPoints;
					}
					ImGui::PopID();
				}

				if(previousPointsChanged) {
					RecalculatePoints();
					changed = true;
				}
			}
			ImGui::PopID();
		}
		return changed;
	}

	bool GenericBezierCurve::OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix)
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;

		glm::mat4 LocalToWorld = Math::Identity<glm::mat4>();
		for(const auto& mat : m_Matrices) LocalToWorld = mat * LocalToWorld;
		LocalToWorld = e.GetComponent<TransformComponent>().GetTransform() * LocalToWorld;
		glm::mat4 WorldToLocal = Math::Inverse(LocalToWorld);
		glm::mat4 matrix;

		bool cpChanged = false;
		for (int i = 0; i < m_Points.size(); ++i) {
			int pointIndex = i % GetTotalControlPoints();
			int controlLine = i/GetTotalControlPoints();

			if(controlLine > 0 && pointIndex < (int)m_Continuity) continue;

			matrix = glm::translate(Math::TransformPoint(LocalToWorld, m_Points[i]));
			ImGuizmo::PushID(i);
			ImGuizmo::Manipulate(viewMatrix, projectionMatrix, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(matrix));

			if(ImGuizmo::IsUsing())
			{
				changed = true;
				Vec3 worldPos = matrix[3];
				m_Points[i] = Math::TransformPoint(WorldToLocal, worldPos);
				ControlPointsChanged();
			}
			ImGuizmo::PopID();
		}

		if(changed) {
			ControlPointsChanged();
		}

		return changed;
	}

	void GenericBezierCurve::ContinuityChanged()
	{
		VXM_PROFILE_FUNCTION();
		RecalculatePoints();
	}

	void GenericBezierCurve::ControlPointsChanged()
	{
		VXM_PROFILE_FUNCTION();
		int amountNeeded = m_Points.size() % GetTotalControlPoints();
		if(amountNeeded != 0) {
			for (int i = 0; i < GetTotalControlPoints() - amountNeeded; ++i) {
				const auto& l1 = m_Points[m_Points.size() - 2];
				const auto& l2 = m_Points[m_Points.size() - 1];
				m_Points.push_back(l2 + l2 - l1);
			}
		}
		RecalculatePoints();
	}

	void GenericBezierCurve::AddPoints()
	{
		VXM_PROFILE_FUNCTION();
		auto controlPointsCount = GetTotalControlPoints();
		auto lineCount = std::max(static_cast<int>(m_Points.size()) / GetTotalControlPoints(), 0);

		int indexStart = lineCount * controlPointsCount;

		if(lineCount == 0)
		{
			for(int i = 0; i < controlPointsCount; ++i)
			{
				m_Points.emplace_back();
			}
		}
		else
		{

			for(int i = 0; i < controlPointsCount; ++i)
			{
				if(i == 0) {
					m_Points.push_back(m_Points[indexStart-1]);
				} else if (i == 1) {
					const auto& l1 = m_Points[indexStart-1];
					const auto& l2 = m_Points[indexStart-2];

					Vec3 dir = l1 - l2;

					m_Points.push_back(l1 + dir);
				} else if (i == 2) {

					const auto& l1 = m_Points[indexStart-1];
					const auto& l2 = m_Points[indexStart-2];
					const auto& l3 = m_Points[indexStart-3];

					Vec3 dir = l1 - l2;

					m_Points.push_back(l3 + (dir * (Real)2.0));
				} else {
					const auto& lst1 = m_Points[m_Points.size() - 1];
					const auto& lst2 = m_Points[m_Points.size() - 2];
					m_Points.push_back(lst2 + (lst2-lst1));
				}
			}
			//TODO: Add all the points so it continuous all the way possible
		}
	}

	void GenericBezierCurve::RecalculatePoints()
	{
		for (int controlPoints = GetTotalControlPoints(); controlPoints < m_Points.size(); controlPoints+=GetTotalControlPoints()) {
			int controlLine = controlPoints/GetTotalControlPoints();
			const int subPointCount = std::min(GetTotalControlPoints(), (int)m_Continuity);
			for(int pointIndex = 0; pointIndex < subPointCount; ++pointIndex) {
				const int index = controlPoints + pointIndex;
				Vec3& point = m_Points[index];
				if (pointIndex == 0) {// Position Continuity
					point = m_Points[controlPoints - 1];
				}
				else if (pointIndex == 1) {// Velocity Continuity
					const Vec3 &last = m_Points[controlPoints - 1];
					const Vec3 &lastMinOne = m_Points[controlPoints - 2];
					Vec3 dir = last - lastMinOne;
					point = last + dir;
				}
				else if (pointIndex == 2) {// Acceleration Continuity
					const auto &l1 = m_Points[controlPoints - 1];
					const auto &l2 = m_Points[controlPoints - 2];
					const auto &l3 = m_Points[controlPoints - 3];

					Vec3 dir = l1 - l2;

					point = l3 + (dir * (Real) 2.0);
				}
			}
		}
	}


	BoundingBox GenericBezierCurve::GetBoundingWorldBox(const Mat4& localToWorld) const
	{
		VXM_PROFILE_FUNCTION();
		return GetWorldPoints(localToWorld);
	}

	BoundingSphere GenericBezierCurve::GetBoundingWorldSphere(const Mat4& localToWorld) const
	{
		VXM_PROFILE_FUNCTION();
		return GetWorldPoints(localToWorld);
	}

	std::vector<Vec3> GenericBezierCurve::GetWorldPoints(const Mat4 &transform) const
	{
		VXM_PROFILE_FUNCTION();
		std::vector<Vec3> result(m_Points.size());
		auto matrix = Math::Identity<glm::mat4>();
		for(const auto& mat : m_Matrices) matrix = mat * matrix;
		matrix = transform * matrix;
		std::transform(m_Points.begin(), m_Points.end(), result.begin(), [&matrix](const Vec3& p) {return Math::TransformPoint(matrix, p);});
		return result;
	}
}// namespace Voxymore::Core