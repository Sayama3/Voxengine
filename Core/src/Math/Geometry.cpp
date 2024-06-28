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

	void ClosestPointOnPointSegment(Vec3 c, Segment seg, Real &t, Vec3 &d)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 a = seg.begin;
		Vec3 b = seg.end;
		Vec3 ab = b - a;

		t = Dot(c - a, ab) / Dot(ab, ab);
		t = Clamp(t, 0.0f, 1.0f);
		d = a + t * ab;
	}

	Real SqDistPointSegment(Segment seg, Vec3 c)
	{
		Vec3 a = seg.begin;
		Vec3 b = seg.end;

		Vec3 ab = b - a;
		Vec3 ac = c - a;
		Vec3 bc = c - b;

		Real e = Dot(ac, ab);
		// c is outside of ab.
		if(e <= 0) {
			return Dot(ac,ac);
		}
		Real f = Dot(ab, ab);
		if(e >= f) {
			return Dot(bc, bc);
		}
		// c is onto ab
		return Dot(ac, ac) - e * e / f;
	}

	void ClosestPointToAABB(Vec3 p, AABB b, Vec3 &q)
	{
		for (int i = 0; i < 3; ++i) {
			q[i] = Clamp(p[i], b.min[i], b.max[i]);
		}
	}
	Real SqDistPointAABB(Vec3 p, AABB b)
	{
		Real sqDist = 0;
		for (int i = 0; i < 3; ++i) {
			Real v = p[i];
			if (v < b.min[i]) {
				sqDist += (b.min[i] - v) * (b.min[i] - v);
			}
			if (v > b.max[i]) {
				sqDist += (v - b.max[i]) * (v - b.max[i]);
			}
		}
		return sqDist;
	}

	void ClosestPointToOBB(Vec3 p, OBB b, Vec3 &q)
	{
		Vec3 d = p - b.c;
		q = b.c;
		for (int i = 0; i < 3; ++i) {
			Real dist = Clamp(Dot(d, b.u[i]), -b.e[i], b.e[i]);
			q += dist * b.u[i];
		}
	}

	Real SqDistPointOBB(Vec3 p, OBB b)
	{
		Real sqDist = 0;
		Vec3 v = p - b.c;
		for (int i = 0; i < 3; ++i) {
			Real d = Dot(v, b.u[i]);
			Real excess = 0;
			if (d < -b.e[i]) {
				excess = d + b.e[i];
			} else if (d > b.e[i]) {
				excess = d - b.e[i];
			}
			sqDist += excess * excess;
		}
		return sqDist;
	}

	// Very similar to OBB but with one axis less
	void ClosestPointToRect(Vec3 p, Rect b, Vec3 &q)
	{
		Vec3 d = p - b.c;
		q = b.c;

		for (int i = 0; i < 2; ++i) {
			q += Clamp(Dot(d, b.u[i]), -b.e[i], b.e[i]) * b.u[i];
		}
	}

	Vec3 ClosestPointToTriangle(Vec3 p, Triangle t)
	{
		VXM_PROFILE_FUNCTION();

		// Computing the closest point using the voronoi regions of the triangle
		//  and the Barycentric Coordinate of the point p in the triangle t;
		Vec3 a = t.a;
		Vec3 b = t.b;
		Vec3 c = t.c;

		Vec3 ab = b - a;
		Vec3 ac = c - a;
		Vec3 bc = c - b;

		// Compute parametric position s for projection P' of P to AB
		//P' = A + s*AB; s= snom/(snom*sdenom)
		Real snom = Dot(p - a, ab);
		Real sdenom = Dot(p - b, a - b);

		Real tnom = Dot(p - a, ac);
		Real tdenom = Dot(p - c, a - c);

		if(snom <= 0 && tnom <= 0) return a; // Vertex region early out.

		Real unom = Dot(p - b, bc);
		Real udenom = Dot(p - c, b - c);

		if(sdenom <= 0 && unom <= 0) return b; // Vertex region early out.
		if(tdenom <= 0 && udenom <= 0) return c; // Vertex region early out.

		// If P outside AB, Triple Scalar Product [N PA PB] <= 0
		Vec3 n = Cross(b - a, c - a);
		Real vc = Dot(n, Cross(a - p, b - p));

		if(vc < 0 && snom >= 0 && sdenom >= 0) {
			return a + snom / (snom + sdenom) * ab;
		}


		// If P outside BC, Triple Scalar Product [N PB PC] <= 0
		Real va = Dot(n, Cross(b - p, c - p));

		if(va < 0 && unom >= 0 && udenom >= 0) {
			return b + unom / (unom + udenom) * bc;
		}

		// If P outside CA, Triple Scalar Product [N PC PA] <= 0
		Real vb = Dot(n, Cross(c - p, a - p));
		if(va < 0 && unom >= 0 && udenom >= 0) {
			return a + tnom / (tnom + tdenom) * ac;
		}

		// P is inside the region. Use barycentric coordinate to compute the result.
		Real u = va / (va + vb + vc);
		Real v = vb / (va + vb + vc);
		Real w = Real(1) - u - v;
		return u * a + v * b + w * c;
	}
	Real ClosestPointOfTwoSegment(Segment seg1, Segment seg2, Real &s, Real &t, Vec3 &c1, Vec3 &c2)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 p1 = seg1.begin;
		Vec3 q1 = seg1.end;
		Vec3 p2 = seg2.begin;
		Vec3 q2 = seg2.end;

		Vec3 d1 = seg1.Direction();
		Vec3 d2 = seg2.Direction();
		Vec3 r = p1 - p2;

		Real a = Dot(d1, d1); // Square Length.
		Real e = Dot(d2, d2); // Square Length.
		Real f = Dot(d2, r);

		if(a <= Epsilon && e <= Epsilon) {
			// Both Segment degenerate into points.
			s = t = 0;
			c1 = p1;
			c2 = p2;
			return Dot(c1 - c2, c1 - c2);
		}
		if (a <= Epsilon) {
			// First Segment Degenerate into point.
			s = 0;
			t = f / e;
			t = Clamp01(t);
		}
		else {
			Real c = Dot(d1, r);
			if (e <= Epsilon) {
				// Second Segment Degenerate into point.
				t = 0;
				s = Clamp01(-c / a);
			} else {
				// General non-degenerate case
				Real b = Dot(d1, d2);
				Real denom = a*e - b*b;

				if(denom != 0) s = Clamp01((c*f - c*e) / denom);
				else s = 0; // Arbitrary pick to avoid NaN.

				// t = Dot((P1 + D1*s) - P2, P2) / Dot(D2,D2) == (b*s + f) / e
				// TODO?: Optimizing the division by deferring it with a `tdenom` until t is known to be in bound.
				t = (b*s + f) / e;

				// Checking if `t` is in Bounds [0, 1], else recomputing a better `s` after clamping `t`.
				if(t < 0) {
					t = 0;
					s = Clamp01(-c/a);
				} else if (t > 1) {
					t = 1;
					s = Clamp01((b - c) / a);
				}
			}
		}

		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;
		return Dot(c1 - c2, c1 - c2);
	}

	Real Signed2DTriArea(Triangle t)
	{
		const Vec3& a = t.a;
		const Vec3& b = t.b;
		const Vec3& c = t.c;
		return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
	}

	bool TestSpherePlane(Sphere s, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(Approx(SqrMagnitude(p.n), 1), "The plane Normal is not normalize.");
		Real dist = Dot(s.c, p.n) - p.d;
		return Abs(dist) <= s.r;
	}

	bool InsideSpherePlane(Sphere s, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(Approx(SqrMagnitude(p.n), 1), "The plane Normal is not normalize.");
		Real dist = Dot(s.c, p.n) - p.d;
		return dist < -s.r;
	}

	bool TestSphereHalfSpace(Sphere s, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(Approx(SqrMagnitude(p.n), 1), "The plane Normal is not normalize.");
		Real dist = Dot(s.c, p.n) - p.d;
		return dist <= s.r;
	}

	bool TestOBBPlane(OBB b, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		// Compute the projection interval radius of b onto L(t) = b.c + t*p.n.
		Real r = b.e[0] * Abs(Dot(p.n, b.u[0])) +
				 b.e[1] * Abs(Dot(p.n, b.u[1])) +
				 b.e[2] * Abs(Dot(p.n, b.u[2]));

		// Distance Box center from plane
		Real s = Dot(p.n, b.c) - p.d;

		// Intersection when s is within [-r, +r]
		return Abs(s) <= r;
	}

	bool TestOBBHalfSpace(OBB b, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		// Compute the projection interval radius of b onto L(t) = b.c + t*p.n.
		Real r = b.e[0] * Abs(Dot(p.n, b.u[0])) +
				 b.e[1] * Abs(Dot(p.n, b.u[1])) +
				 b.e[2] * Abs(Dot(p.n, b.u[2]));

		// Distance Box center from plane
		Real s = Dot(p.n, b.c) - p.d;

		// Intersection when s is within [-r, +r]
		return s <= -r;
	}

	bool TestAABBPlane(AABB b, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		// Compute the projection interval radius of b onto L(t) = b.c + t*p.n.
		Vec3 c = b.Center();
		Vec3 e = b.max - c;

		// Distance Box center from plane
		Real r = e[0] * Abs(p.n[0]) +  e[1] * Abs(p.n[1]) +  e[2] * Abs(p.n[2]);
		Real s = Dot(p.n, c) - p.d;

		// Intersection when s is within [-r, +r]
		return Abs(s) <= r;
	}

	bool TestAABBHalfSpace(AABB b, Plane p)
	{
		VXM_PROFILE_FUNCTION();
		// Compute the projection interval radius of b onto L(t) = b.c + t*p.n.
		Vec3 c = b.Center();
		Vec3 e = b.max - c;

		// Distance Box center from plane
		Real r = e[0] * Abs(p.n[0]) +  e[1] * Abs(p.n[1]) +  e[2] * Abs(p.n[2]);
		Real s = Dot(p.n, c) - p.d;

		// Intersection when s is within [-r, +r]
		return s <= -r;
	}

	bool TestSphereAABB(Sphere s, AABB b)
	{
		VXM_PROFILE_FUNCTION();
		Real sqDist = SqDistPointAABB(s.c, b);
		return sqDist <= Pow2(s.r);
	}

	bool Math::TestSphereAABB(Sphere s, AABB b, Vec3 &p)
	{
		VXM_PROFILE_FUNCTION();
		ClosestPointToAABB(s.c, b, p);
		Vec3 v = p - s.c;
		return Dot(v,v) <= Pow2(s.r);
	}

	bool TestSphereOBB(Sphere s, OBB b)
	{
		VXM_PROFILE_FUNCTION();
		Real sqDist = SqDistPointOBB(s.c, b);
		return sqDist <= Pow2(s.r);
	}
	bool Math::TestSphereOBB(Sphere s, OBB b, Vec3 &p)
	{
		VXM_PROFILE_FUNCTION();
		ClosestPointToOBB(s.c, b, p);
		Vec3 v = p - s.c;
		return Dot(v,v) <= Pow2(s.r);
	}

	bool TestSphereTriangle(Sphere s, Triangle t, Vec3 &p)
	{
		VXM_PROFILE_FUNCTION();
		p = ClosestPointToTriangle(s.c, t);
		Vec3 v = p - s.c;
		return Dot(v, v) <= Pow2(s.r);
	}

	bool TestTriangleAABB(Triangle v, AABB b)
	{
		Real p0, p1, p2, r;

		Vec3 c = b.Center();
		Real e0 = (b.max[0] - b.min[0]) * Real(0.5);
		Real e1 = (b.max[1] - b.min[1]) * Real(0.5);
		Real e2 = (b.max[2] - b.min[2]) * Real(0.5);

		// Recompute triangle to AABB Origin.
		v = { v[0] - c, v[1] - c, v[2] - c};

		Vec3 f0 = v[1]- v[0];
		Vec3 f1 = v[2]- v[1];
		Vec3 f2 = v[0]- v[2];

		// TODO: Check the SAT a00 to a22
		// Test Axis a00
		p0 = v[0].z*v[1].y - v[0].y*v[1].z;
		p2 = v[2].z*(v[1].y - v[0].y) - v[2].y*(v[1].z - v[0].z);
		r = e1 * Abs(f0.z) + e2 * Abs(f0.y);
		if(Max(-Max(p0,p2), Min(p0,p2)) < r) return false;

		// Axis a01
		p0 = v[0].z * v[1].x - v[0].x * v[1].z;
		p2 = v[2].z * (v[1].x - v[0].x) - v[2].x * (v[1].z - v[0].z);
		r = e0 * Abs(f1.z) + e2 * Abs(f1.x);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;

		// Axis a02
		p0 = v[0].y * v[1].x - v[0].x * v[1].y;
		p2 = v[2].y * (v[1].x - v[0].x) - v[2].x * (v[1].y - v[0].y);
		r = e0 * Abs(f2.y) + e1 * Abs(f2.x);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;

		// Test axis a10
		p0 = v[0].z * v[2].y - v[0].y * v[2].z;
		p2 = v[1].z * (v[2].y - v[0].y) - v[1].y * (v[2].z - v[0].z);
		r = e1 * Abs(f2.z) + e2 * Abs(f2.y);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;
		
		// Test axis a11
		p0 = v[0].z * v[2].x - v[0].x * v[2].z;
		p2 = v[1].z * (v[2].x - v[0].x) - v[1].x * (v[2].z - v[0].z);
		r = e0 * Abs(f0.z) + e2 * Abs(f0.x);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;

		// Test axis a12
		p0 = v[0].y * v[2].x - v[0].x * v[2].y;
		p2 = v[1].y * (v[2].x - v[0].x) - v[1].x * (v[2].y - v[0].y);
		r = e0 * Abs(f1.y) + e1 * Abs(f1.x);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;
		
    
		//Test axis a20
		p0 = v[1].z * v[2].y - v[1].y * v[2].z;
		p2 = v[0].z * (v[2].y - v[1].y) - v[0].y * (v[2].z - v[1].z);
		r = e1 * Abs(f1.z) + e2 * Abs(f1.y);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;
		
		//Test axis a21
		p0 = v[1].z * v[2].x - v[1].x * v[2].z;
		p2 = v[0].z * (v[2].x - v[1].x) - v[0].x * (v[2].z - v[1].z);
		r = e0 * Abs(f2.z) + e2 * Abs(f2.x);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;

		//Test axis a22
		p0 = v[1].y * v[2].x - v[1].x * v[2].y;
		p2 = v[0].y * (v[2].x - v[1].x) - v[0].x * (v[2].y - v[1].y);
		r = e0 * Abs(f0.y) + e1 * Abs(f0.x);
		if (Max(-Max(p0, p2), Min(p0, p2)) > r) return false;

		if(Max(v[0].x, v[1].x, v[2].x) < -e0 || Min(v[0].x, v[1].x, v[2].x) > e0 ) return false;
		if(Max(v[0].y, v[1].y, v[2].y) < -e1 || Min(v[0].y, v[1].y, v[2].y) > e1 ) return false;
		if(Max(v[0].z, v[1].z, v[2].z) < -e2 || Min(v[0].z, v[1].z, v[2].z) > e2 ) return false;

		Plane p;
		p.n = Cross(f0, f1);
		p.d = Dot(p.n, v[0]);
		return TestAABBPlane(b ,p);
	}
} // namespace Voxymore::Core::Math

