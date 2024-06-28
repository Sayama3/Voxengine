//
// Created by ianpo on 24/06/2024.
//

#pragma once

#include "Math.hpp"

namespace Voxymore::Core::Math
{
	// Precisely a 3d rect
	struct Rect
	{
		Rect() = default;
		~Rect() = default;

		Rect(Vec3 center, Vec2 halfExtents, const Mat3x2 &localAxes) : c(center), matrix(localAxes), halfExtents(halfExtents) {}
		Rect(Vec3 center, Vec2 halfExtents, const std::array<Vec3, 2> &localAxes) : c(center), axes(localAxes), halfExtents(halfExtents) {}
		Rect(Vec3 center, Real width, Real height, const Mat3x2 &localAxes) : c(center), matrix(localAxes), halfExtents(width, height) {}
		Rect(Vec3 center, Real width, Real height, const std::array<Vec3, 2> &localAxes) : c(center), axes(localAxes), halfExtents(width, height) {}
		Rect(Vec3 a, Vec3 b, Vec3 c);

	public:
		Vec3 c;

		union {
			Vec3 u[2];
			std::array<Vec3, 2> axes;
			Mat3x2 matrix;
		};

		union {
			Real e[2];
			Vec2 halfExtents;
			struct {Real halfWidth; Real halfHeight;};
		};
	};

} // namespace Voxymore::Core::Math


