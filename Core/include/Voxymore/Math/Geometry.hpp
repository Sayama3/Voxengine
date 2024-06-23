//
// Created by ianpo on 23/06/2024.
//

#pragma once

#include "Math.hpp"
#include "AABB.hpp"
#include "OBB.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Capsule.hpp"

namespace Voxymore::Core::Math
{
	static bool IsConvexQuad(Vec3 a, Vec3 b, Vec3 c, Vec3 d);
	static Vec3 ClosestPointOnPlaneToPoint(Vec3 q, Plane p);
	static Real DistPointPlane(Vec3 q, Plane p);

	// Given Segment ab and point c, compute closest point d on ab.
	// Also return t for the position of d, d(t) = a + t * (b - a);
	static void ClosestPointOnPointSegment(Vec3 c, Vec3 a, Vec3 b, Real& t, Vec3& d);

} // namespace Voxymore::Core::Math


