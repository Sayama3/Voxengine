//
// Created by ianpo on 24/06/2024.
//

#pragma once

#include "Math.hpp"

namespace Voxymore::Core::Math
{

	struct Triangle
	{
		using iterator = std::array<Vec3, 3>::iterator;
		using const_iterator = std::array<Vec3, 3>::const_iterator;

		Triangle() = default;
		~Triangle() = default;
		Triangle(Vec3 A, Vec3 B, Vec3 C) : a(A), b(B), c(C) {}
		Triangle(std::array<Vec3, 3> pts) : points(pts) {}

		union {
			struct {Vec3 a; Vec3 b; Vec3 c;};
			std::array<Vec3, 3> points;
			Vec3 p[3];
		};

		iterator begin() {return points.begin();}
		const_iterator begin() const {return points.begin();}
		const_iterator cbegin() const {return points.begin();}

		iterator end() {return points.end();}
		const_iterator end() const {return points.end();}
		const_iterator cend() const {return points.end();}


		Vec3& operator[](uint64_t i) {return points[i];}
		const Vec3& operator[](uint64_t i) const {return points[i];}

	};

} // namespace Voxymore::Core::Math


