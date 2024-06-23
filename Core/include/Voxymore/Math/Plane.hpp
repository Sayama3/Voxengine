//
// Created by ianpo on 23/06/2024.
//

#pragma once

#include "Math.hpp"

namespace Voxymore::Core::Math
{

	/**
	 * @struct Plane
	 * @brief Represents a plane in 3D space.
	 *
	 * The Plane struct defines a plane using its normal vector (n) and the distance (d)
	 * from the origin along the normal vector. The plane equation is defined as:
	 *     n.x * x + n.y * y + n.z * z = d
	 *
	 * The Plane struct is typically used for various geometric calculations involving planes,
	 * such as intersection tests, ray-plane intersections, etc.
	 */
	struct Plane
	{
		// Plane Normal
		Vec3 n;
		// d = Dot(n,p); for a given point p on the plane.
		Real d;


		/**
		 * @brief Computes the plane defined by three points in 3D space.
		 *
		 * This function calculates the plane defined by three given points. It returns a Plane structure
		 * that represents the computed plane. The plane is defined by its normal vector (n) and the
		 * distance (d) from the origin along the normal vector.
		 *
		 * @param a The first point in 3D space.
		 * @param b The second point in 3D space.
		 * @param c The third point in 3D space.
		 * @return The Plane structure representing the computed plane.
		 *
		 * @see Plane
		 * @see Vec3
		 * @see Normalize
		 * @see Cross
		 * @see Dot
		 */
		static Plane Compute(Vec3 a, Vec3 b, Vec3 c);
	};


} // namespace Voxymore::Core::Math


