//
// Created by Sayama on 06/03/2024.
//

#pragma once

#include "Voxymore/Math/Math.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	class Primitive
	{
	public:
		Rigidbody* m_Body = nullptr;
		Mat4 m_Offset = Math::Identity<Mat4>();

		[[nodiscard]] virtual Vec3 GetPosition() const;

		/**
		 * @brief This function returns the axis (i.e. a column of the matrix) of the Primitive object at a given index.
		 *
		 * @param i The index of the axis.
		 * @return Vec3 The axis vector.
		 *
		 * @note The function assumes that index `i` is non-negative and within the range of the offset length.
		 * If the provided index is invalid, the behavior is undefined.
		 */
		[[nodiscard]] Vec3 GetAxis(int32_t i) const;
	};

	class Sphere : public Primitive
	{
	public:
		Real m_Radius = 1;
	};

	class Plane : public Primitive
	{
	public:
		Vec3 m_Normal = {0,0,0};
		Real m_Offset = 0;
	};

	class Box : public Primitive
	{
	public:
		Vec3 m_HalfSize = {0,0,0};

		[[nodiscard]] std::array<Vec3, 8> GetVertices() const;
	};
} // namespace Voxymore::Core
