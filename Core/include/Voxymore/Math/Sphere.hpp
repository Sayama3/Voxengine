//
// Created by ianpo on 23/06/2024.
//

#pragma once

#include "Math.hpp"

namespace Voxymore::Core::Math
{
	struct Sphere
	{
		Vec3 c;
		Real r;

		static bool Overlap(Sphere a, Sphere b);
	};
}// namespace Voxymore::Core::Math
