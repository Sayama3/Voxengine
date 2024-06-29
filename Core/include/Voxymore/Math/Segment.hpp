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
		Segment(Vec3 b, Vec3 e) : begin(b), end(e) {}

		[[nodiscard]] Vec3 Direction() const { return end - begin;}

		union {
			struct {Vec3 begin; Vec3 end;};
			std::array<Vec3, 2> points;
			Vec3 rawPoints[2];
		};
	};

	struct Ray {
		Ray() = default;
		~Ray() = default;

		Ray(Vec3 origin, Vec3 direction) : origin(origin), direction(Normalize(direction)) {}
		Ray(Segment seg) : origin(seg.begin), direction(Normalize(seg.end - seg.begin)) {}

		union {
			struct {Vec3 origin; Vec3 direction;};
			std::array<Vec3, 2> data;
		};

		Vec3 GetPoint(Real t) { return origin + t * direction; }
	};
}
