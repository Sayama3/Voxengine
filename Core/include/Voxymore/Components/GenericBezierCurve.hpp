//
// Created by Iannis on 02/04/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include "Voxymore/Math/BoundingSphere.hpp"
#include "Voxymore/Components/CustomComponent.hpp"

namespace Voxymore::Core
{
	class GenericBezierCurve : public SelfAwareComponent<GenericBezierCurve>
	{
		VXM_IMPLEMENT_NAME(GenericBezierCurve);
	public:
		enum class Continuity {
			None = 0,
			Position,
			Velocity,
			Acceleration,
		};
	public:
		inline GenericBezierCurve() = default;
		inline ~GenericBezierCurve() = default;
		void DeserializeComponent(YAML::Node& node, Entity e);
		void SerializeComponent(YAML::Emitter& out, Entity e);
		bool OnImGuiRender(Entity e);
		bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix);

		[[nodiscard]] inline int GetTotalControlPoints() const {return 2 + std::max(m_ControlPoints, 0);}
		[[nodiscard]] BoundingBox GetBoundingWorldBox(const Mat4& localToWorld) const;
		[[nodiscard]] BoundingSphere GetBoundingWorldSphere(const Mat4& localToWorld) const;
		[[nodiscard]] std::vector<Vec3> GetWorldPoints(const Mat4& transform) const;
		inline std::string GetShaderName() const { return "Bezier";}
	private:
		void ContinuityChanged();
		void ControlPointsChanged();
		void AddPoints();
		void RecalculatePoints();
	public:
		int m_ControlPoints = 2;
		int Definition = 1000;
		std::string ShaderName = "Bezier";
		Continuity m_Continuity = Continuity::Position;
		std::vector<Vec3> m_Points = {};
		std::vector<glm::mat4> m_Matrices = {};
	};

	inline YAML::Emitter& operator<<(YAML::Emitter &out, const GenericBezierCurve::Continuity& c)
	{
		switch (c) {
			case Voxymore::Core::GenericBezierCurve::Continuity::None: out << "Continuity::None"; break;
			case Voxymore::Core::GenericBezierCurve::Continuity::Position: out << "Continuity::Position"; break;
			case Voxymore::Core::GenericBezierCurve::Continuity::Velocity: out << "Continuity::Velocity"; break;
			case Voxymore::Core::GenericBezierCurve::Continuity::Acceleration: out << "Continuity::Acceleration"; break;
		}
		return out;
	}

	VXM_CREATE_COMPONENT(GenericBezierCurve);
} // namespace Voxymore::Core

namespace YAML
{

	template<>
	struct convert<Voxymore::Core::GenericBezierCurve::Continuity>
	{
		inline static Node encode(const Voxymore::Core::GenericBezierCurve::Continuity& rhs)
		{
			Node node;
			switch (rhs) {
				case Voxymore::Core::GenericBezierCurve::Continuity::None: node.push_back("Continuity::None"); break;
				case Voxymore::Core::GenericBezierCurve::Continuity::Position: node.push_back("Continuity::Position"); break;
				case Voxymore::Core::GenericBezierCurve::Continuity::Velocity: node.push_back("Continuity::Velocity"); break;
				case Voxymore::Core::GenericBezierCurve::Continuity::Acceleration: node.push_back("Continuity::Acceleration"); break;
			}
			return node;
		}

		inline static bool decode(const Node& node, Voxymore::Core::GenericBezierCurve::Continuity& rhs)
		{
			auto str = node.as<std::string>(std::string());
			if (str == "Continuity::None") rhs = Voxymore::Core::GenericBezierCurve::Continuity::None;
			else if (str == "Continuity::Position") rhs = Voxymore::Core::GenericBezierCurve::Continuity::Position;
			else if (str == "Continuity::Velocity") rhs = Voxymore::Core::GenericBezierCurve::Continuity::Velocity;
			else if (str == "Continuity::Acceleration") rhs = Voxymore::Core::GenericBezierCurve::Continuity::Acceleration;
			else return false;

			return true;
		}
	};
}


