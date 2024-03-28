//
// Created by ianpo on 25/03/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Components/CustomComponent.hpp"

namespace Voxymore
{
	namespace Core
	{

		struct BezierCurve : public SelfAwareComponent<BezierCurve>
		{
			VXM_IMPLEMENT_NAME(BezierCurve);
		public:
			BezierCurve() = default;
			~BezierCurve() = default;

			void DeserializeComponent(YAML::Node& node, Entity e);
			void SerializeComponent(YAML::Emitter& out, Entity e);
			bool OnImGuiRender(Entity e);
			bool OnImGuizmo(Entity e, const float* viewMatrix, const float* projectionMatrix);

			std::array<Vec3, 4> LocalControlPoints = {Vec3{-1,0,0}, Vec3{0,1,1}, Vec3{0,-1,-1}, Vec3{1,0,0}};
		};

		VXM_CREATE_COMPONENT(BezierCurve)

	}// namespace Core
}// namespace Voxymore
