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
}
