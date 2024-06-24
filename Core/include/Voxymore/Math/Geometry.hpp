//
// Created by ianpo on 23/06/2024.
//

#pragma once

#include "Math.hpp"
#include "AABB.hpp"
#include "OBB.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Segment.hpp"
#include "Capsule.hpp"
#include "Rect.hpp"
#include "Triangle.hpp"

namespace Voxymore::Core::Math
{
	static bool IsConvexQuad(Vec3 a, Vec3 b, Vec3 c, Vec3 d);
	static Vec3 ClosestPointOnPlaneToPoint(Vec3 q, Plane p);
	static Real DistPointPlane(Vec3 q, Plane p);

	// Given Segment ab and point c, compute closest point d on ab.
	// Also return t for the position of d, d(t) = a + t * (b - a);
	static void ClosestPointOnPointSegment(Vec3 c, Segment seg, Real& t, Vec3& d);

	[[nodiscard]] static Real SqDistPointSegment(Segment seg, Vec3 c);

	static void ClosestPointToAABB(Vec3 p, AABB b, Vec3& q);
	[[nodiscard]] static Real SqDistPointAABB(Vec3 p, AABB b);

	static void ClosestPointToOBB(Vec3 p, OBB b, Vec3& q);
	[[nodiscard]] static Real SqDistPointOBB(Vec3 p, OBB b);

	static void ClosestPointToRect(Vec3 p, Rect b, Vec3& q);

	[[nodiscard]] static Vec3 ClosestPointToTriangle(Vec3 p, Triangle t);

	// Computes closest points C1 & C2 of S1(s) = seg1.begin + s *(seg1.end - seg1.begin); and
	// S2(t) = seg2.begin + t *(seg2.end - seg2.begin);
	// the Result is the squared distance between S1(s) & S2(t).
	static Real ClosestPointOfTwoSegment(Segment seg1, Segment seg2, Real& s, Real& t, Vec3& c1, Vec3& c2);

	// Return 2 times the signed triangle area. The result is positive if
	//  abc is ccw, negative if abc is cw, zero is abc is degenerate.
	static Real Signed2DTriArea(Triangle t);

} // namespace Voxymore::Core::Math


