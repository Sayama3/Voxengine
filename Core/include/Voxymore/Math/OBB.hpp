//
// Created by ianpo on 23/06/2024.
//

#pragma once

#include "Math.hpp"
#include "AABB.hpp"



namespace Voxymore::Core::Math
{

	struct OBB
	{
		OBB() = default;
		~OBB() = default;
		OBB(AABB a) : c(a.Center()), mat(Identity<Mat3>()), e(a.HalfExtents()) {}
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
		struct Extent {
			Real halfWidth; Real halfHeight; Real halfDepth;
		};
		union {
			Vec3 e;
			Extent extent;
		};

		static bool Overlap(const OBB& a, const OBB& b);
	};

} // namespace Voxymore::Core::Math


