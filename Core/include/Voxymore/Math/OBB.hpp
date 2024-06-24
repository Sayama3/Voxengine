//
// Created by ianpo on 23/06/2024.
//

#pragma once

#include "Math.hpp"



namespace Voxymore::Core::Math
{

	struct OBB
	{
		OBB() = default;
		~OBB() = default;
		OBB(Vec3 center, Mat3 localAxes, Vec3 halfExtents) : c(center), mat(localAxes), e(halfExtents) {}
		OBB(Vec3 center, std::array<Vec3, 3> localAxes, Vec3 halfExtents) : c(center), axes(localAxes), e(halfExtents) {}
		// Center
		Vec3 c;

		static_assert(sizeof(Vec3)*3 == sizeof(Mat3));

		// Local x- ,y-, z- axes
		union {
			Vec3 u[3];
			Mat3 mat;
			std::array<Vec3, 3> axes;
		};

		// Positive halfwidth extents of OBB along each axis.
		Vec3 e;

		static bool Overlap(const OBB& a, const OBB& b);
	};

} // namespace Voxymore::Core::Math


