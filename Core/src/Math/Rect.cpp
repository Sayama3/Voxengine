//
// Created by ianpo on 24/06/2024.
//

#include "Voxymore/Math/Rect.hpp"

namespace Voxymore::Core::Math
{
	Rect::Rect(Vec3 a, Vec3 b, Vec3 c)
	{
		VXM_PROFILE_FUNCTION();
		u[0] = b - a;
		u[1] = c - a;

		this->c = a + (u[0] * Real(0.5)) + (u[1] * Real(0.5));

		e[0] = Math::Magnitude(u[0]);
		e[1] = Math::Magnitude(u[1]);

		u[0] /= e[0];
		u[1] /= e[1];
	}
}// namespace Voxymore::Core::Math