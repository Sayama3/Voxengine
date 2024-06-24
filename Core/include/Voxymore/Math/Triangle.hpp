//
// Created by ianpo on 24/06/2024.
//

#pragma once

#include "Math.hpp"

namespace Voxymore::Core::Math
{

	struct Triangle
	{
		Triangle() = default;
		~Triangle() = default;
		Triangle(Vec3 A, Vec3 B, Vec3 C) : a(A), b(B), c(C) {}
		Triangle(std::array<Vec3, 3> pts) : points(pts) {}

		union {
			struct {Vec3 a; Vec3 b; Vec3 c;};
			std::array<Vec3, 3> points;
			Vec3 p[3];
		};

	};

} // namespace Voxymore::Core::Math


