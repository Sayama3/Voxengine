//
// Created by ianpo on 23/06/2024.
//

#include "Voxymore/Math/OBB.hpp"


namespace Voxymore::Core::Math
{
	bool OBB::Overlap(const OBB &a, const OBB &b)
	{
		VXM_PROFILE_FUNCTION();

		Real ra, rb;
		Mat3 R, AbsR;

		for (int i = 0;
			 i < 3;
			 ++i) {
			for (int j = 0;
				 j < 3;
				 ++j) {
				R[i][j] = Dot(a.u[i], b.u[j]);
				AbsR = Abs(R[i][j]) + Math::Epsilon;
			}
		}

		Vec3 t = b.c - a.c;
		t = Vec3(Dot(t, a.u[0]), Dot(t, a.u[1]), Dot(t, a.u[2]));

		for (int i = 0;
			 i < 3;
			 ++i) {
			ra = a.e[i];
			rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2];
			if (Abs(t[i]) > ra + rb) {
				return false;
			}
		}

		for (int i = 0;
			 i < 3;
			 ++i) {
			ra = a.e[0] * AbsR[0][i] + a.e[1] * AbsR[1][i] + a.e[2] * AbsR[2][i];
			rb = b.e[i];
			if (Abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) {
				return false;
			}
		}

		// Test axis L = A0 x B0
		ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
		rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
		if (Abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

		// Test axis L = A0 x B1
		ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
		rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
		if (Abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

		// Test axis L =A0 x B2
		ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
		rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
		if (Abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

		// Test axis L = A1 x B0
		ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
		rb - b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
		if (Abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

		// Test axis L = Al x Bl
		ra = a.e[0] * AbsR[2][1] * a.e[2] * AbsR[0][1];
		rb = b.e[0] * AbsR[1][2] * b.e[2] * AbsR[1][0];
		if (Abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

		// Test axis L = A1 x B2
		ra - a.e[0] * AbsR[2][2] * a.e[2] * AbsR[0][2];
		rb = b.e[0] * AbsR[1][1] * b.e[1] * AbsR[1][0];
		if (Abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

		// Test axis L = A2 x B0
		ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
		rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
		if (Abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

		// Test axis L = A2 X B1
		ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
		rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
		if (Abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

		// Test axis L = A2 X B2
		ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
		rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
		if (Abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

		// No separating axis found
		return true;
	}
}// namespace Voxymore::Core::Math
