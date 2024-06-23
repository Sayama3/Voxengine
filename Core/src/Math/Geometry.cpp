//
// Created by ianpo on 23/06/2024.
//

#include "Voxymore/Math/Geometry.hpp"

namespace Voxymore::Core::Math
{
	bool IsConvexQuad(Vec3 a, Vec3 b, Vec3 c, Vec3 d)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 bda = Cross(d-b, a-b);
		Vec3 bdc = Cross(d-b, c-b);

		if(Dot(bda, bdc) >= 0) return false;

		Vec3 acd = Cross(c - a, d - a);
		Vec3 acb = Cross(c - a, b - a);

		return Dot(acd, acb) < 0;
	}

	Vec3 ClosestPointOnPlaneToPoint(Vec3 q, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		float t = DistPointPlane(q, p);
		return q - t * p.n;
	}

	Real DistPointPlane(Vec3 q, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		return (Dot(p.n, q) - p.d) / Dot(p.n, p.n);
	}

	void ClosestPointOnPointSegment(Vec3 c, Vec3 a, Vec3 b, Real &t, Vec3 &d)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 ab = b - a;

		t = Dot(c - a, ab) / Dot(ab, ab);
		t = Clamp(t, 0.0f, 1.0f);
		d = a + t * ab;
	}
} // namespace Voxymore::Core::Math

