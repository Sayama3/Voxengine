//
// Created by ianpo on 24/06/2024.
//

#pragma once

#include "Math.hpp"

namespace Voxymore::Core::Math
{
	struct Segment
	{
		Segment() = default;
		~Segment() = default;
		Segment(const Segment&) = default;
		Segment& operator=(const Segment&) = default;
		Segment(Vec3 b, Vec3 e) : seg(b,e) {}

		[[nodiscard]] Vec3 Direction() const { return seg.end - seg.begin;}

		struct Orientation {
			Vec3 begin; Vec3 end;
		};
		union {
			Orientation seg;
			std::array<Vec3, 2> points;
			Vec3 rawPoints[2];
		};
	};

	struct Ray {
		Ray() = default;
		~Ray() = default;

		Ray(Vec3 origin, Vec3 direction) : ray(origin,Normalize(direction)) {}
		Ray(Segment seg) : ray(seg.seg.begin,Normalize(seg.seg.end - seg.seg.begin)) {}

		struct Orientation {
			Vec3 origin; Vec3 direction;
		};
		union {
			Orientation ray;
			std::array<Vec3, 2> data;
		};

		Vec3 GetPoint(Real t) { return ray.origin + t * ray.direction; }
	};
}
