//
// Created by ianpo on 25/03/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/BoundingBox.hpp"
#include "Voxymore/Math/BoundingSphere.hpp"
#include "Voxymore/Components/CustomComponent.hpp"

namespace Voxymore::Core
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

		inline BoundingBox GetBoundingBox() const { VXM_PROFILE_FUNCTION(); return BoundingBox{LocalControlPoints}; }
		inline BoundingSphere GetBoundingSphere() const { VXM_PROFILE_FUNCTION(); return BoundingSphere{LocalControlPoints}; }

		[[nodiscard]] BoundingBox GetBoundingWorldBox(const Mat4& localToWorld) const;
		[[nodiscard]] BoundingSphere GetBoundingWorldSphere(const Mat4& localToWorld) const;

		std::array<Vec3, 4> LocalControlPoints = {Vec3{-1,0,0}, Vec3{0,1,1}, Vec3{0,-1,-1}, Vec3{1,0,0}};
		int Definition = 1000;
	};

	VXM_CREATE_COMPONENT(BezierCurve)

} // namespace Voxymore::Core

