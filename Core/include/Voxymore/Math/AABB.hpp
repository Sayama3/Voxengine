//
// Created by ianpo on 23/06/2024.
//

#pragma once

#include "Math.hpp"

namespace Voxymore::Core::Math
{

	struct AABB
	{
		Vec3 min;
		Vec3 max;

		static bool Overlap(AABB a, AABB b);
	};

} // namespace Voxymore::Core::Math


