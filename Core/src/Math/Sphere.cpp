//
// Created by ianpo on 23/06/2024.
//

#include "Voxymore/Math/Sphere.hpp"

namespace Voxymore::Core::Math
{
			bool Sphere::Overlap(Sphere a, Sphere b)
			{
				Vec3 d = a.c - b.c;
				Real dist2 = Dot(d,d);
				Real radiusSum = a.r + b.r;
				return dist2 <= radiusSum * radiusSum;
			}
}// namespace Voxymore::Core::Math
