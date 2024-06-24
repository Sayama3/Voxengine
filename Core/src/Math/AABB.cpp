//
// Created by ianpo on 23/06/2024.
//

#include "Voxymore/Math/AABB.hpp"



namespace Voxymore::Core::Math
{
	bool AABB::Overlap(AABB a, AABB b)
	{
		return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
			   (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
			   (a.min.z <= b.max.z && a.max.z >= b.min.z);
	}
	Vec3 AABB::Center() const
	{
		return (min + max) * 0.5f;
	}

	Vec3 AABB::Size() const
	{
		return max - min;
	}

	Vec3 AABB::HalfExtents() const
	{
		return Size() * 0.5f;
	}
} // namespace Voxymore::Core::Math

