//
// Created by ianpo on 25/03/2024.
//

#include "Voxymore/Components/BezierCurve.hpp"
#include "ImGuizmo.h"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore
{
	namespace Core
	{
		void BezierCurve::DeserializeComponent(YAML::Node &node, Entity e)
		{
			VXM_PROFILE_FUNCTION();
			static const std::array<Vec3, 4> defaultPoints = {Vec3{-1,0,0}, Vec3{0,1,1}, Vec3{0,-1,-1}, Vec3{1,0,0}};
			auto controlPointsNode = node["ControlPoints"];
			if(controlPointsNode && controlPointsNode.IsSequence())
			{
				for (int i = 0; i < std::min((int)controlPointsNode.size(),4); ++i) {
					LocalControlPoints[i] = controlPointsNode[i].as<Vec3>(defaultPoints[i]);
				}
			}

			if(node["Definition"]){
				Definition = node["Definition"].as<int>(1000);
			}

		}

		void BezierCurve::SerializeComponent(YAML::Emitter &out, Entity e)
		{
			VXM_PROFILE_FUNCTION();
			out << KEYVAL("ControlPoints", YAML::BeginSeq);
			for(auto& cp : LocalControlPoints)
			{
				out << cp;
			}
			out << YAML::EndSeq;

			out << KEYVAL("Definition",Definition);
		}

		bool BezierCurve::OnImGuiRender(Entity e)
		{
			VXM_PROFILE_FUNCTION();
			bool changed = false;
			changed |= ImGuiLib::DrawVec3Control("Control Point 1", LocalControlPoints[0]);
			changed |= ImGuiLib::DrawVec3Control("Control Point 2", LocalControlPoints[1]);
			changed |= ImGuiLib::DrawVec3Control("Control Point 3", LocalControlPoints[2]);
			changed |= ImGuiLib::DrawVec3Control("Control Point 4", LocalControlPoints[3]);
			changed |= ImGui::DragInt("Definition", &Definition, 1, 0, INT_MAX);
			return changed;
		}

		bool BezierCurve::OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix)
		{
			VXM_PROFILE_FUNCTION();
			bool changed = false;
			glm::mat4 selfMat = e.GetComponent<TransformComponent>().GetTransform();
			glm::mat4 matrix;
			for (int i = 0; i < 4; ++i) {
				matrix = glm::translate(Math::TransformPoint(selfMat,LocalControlPoints[i]));
				ImGuizmo::PushID(i);
				bool result = ImGuizmo::Manipulate(viewMatrix, projectionMatrix, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(matrix));
				changed |= result;

				if(ImGuizmo::IsUsing())
				{
					Vec3 worldPos = matrix[3];
					LocalControlPoints[i] = Math::TransformPoint(glm::inverse(selfMat), worldPos);
				}
				ImGuizmo::PopID();
			}
			return changed;
		}

		BoundingBox BezierCurve::GetBoundingWorldBox(const Mat4& localToWorld) const
		{
			VXM_PROFILE_FUNCTION();
			std::array<Vec3, 4> worldPoints = LocalControlPoints;
			for (auto& p : worldPoints) {
				p = Math::TransformPoint(localToWorld, p);
			}
			return worldPoints;
		}

		BoundingSphere BezierCurve::GetBoundingWorldSphere(const Mat4& localToWorld) const
		{
			VXM_PROFILE_FUNCTION();
			std::array<Vec3, 4> worldPoints = LocalControlPoints;
			for (auto& p : worldPoints) {
				p = Math::TransformPoint(localToWorld, p);
			}
			return worldPoints;
		}
	}// namespace Core
}// namespace Voxymore