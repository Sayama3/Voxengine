//
// Created by Sayama on 06/03/2024.
//

#pragma once

#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	class Rigidbody;

	class PrimitiveCollider
	{
	public:
		enum Type {
			Sphere,
			Plane,
			Box,
		};
	public:
		Rigidbody* m_Body = nullptr;
		TransformComponent* m_Transform = nullptr;

		void CacheMatrix();
		[[nodiscard]] virtual Vec3 GetPosition() const;
		[[nodiscard]] virtual Mat4 GetMatrix() const;

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
	protected:
		Mat4 m_CachedMatrix = Math::Identity<Mat4>();
	};

	class Sphere : public PrimitiveCollider
	{
	public:
		Real m_Radius = .5;
	};

	class Plane : public PrimitiveCollider
	{
	public:
		Vec3 m_Normal = {0,1,0};
		Real m_Offset = 0;
		Real m_Size = 1;
	};

	class Box : public PrimitiveCollider
	{
	public:
		Vec3 m_HalfSize = {.5,.5,.5};

		[[nodiscard]] std::array<Vec3, 8> GetVertices() const;
	};
} // namespace Voxymore::Core
