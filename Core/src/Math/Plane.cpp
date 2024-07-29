//
// Created by ianpo on 23/06/2024.
//

#include "Voxymore/Math/Plane.hpp"

namespace Voxymore
{
	namespace Core
	{
		namespace Math
		{
			Plane Plane::Compute(Vec3 a, Vec3 b, Vec3 c)
			{
				Plane p;
				p.n = Normalize(Cross(b-a, c-a));
				p.d = Dot(p.n, a);
				return p;
			}
		}// namespace Math
	}// namespace Core
}// namespace Voxymore