//
// Created by Sayama on 06/03/2024.
//

#pragma once

#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Math/Geometry.hpp"
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
		Math::Sphere AsSphere() {return Math::Sphere{GetPosition(), m_Radius};}
	public:
		Real m_Radius = .5;
	};

	class Plane : public PrimitiveCollider
	{
	public:
		Math::Plane AsPlane() {return Math::Plane{m_Normal, m_Offset};}
	public:
		Vec3 m_Normal = {0,1,0};
		Real m_Offset = 0;
		//TODO: Replace the size of the plane by a Vec2
		Vec2 m_Size = {1, 1};
	};

	class Box : public PrimitiveCollider
	{
	public:
		Vec3 m_HalfSize = {.5,.5,.5};

		Math::OBB AsOBB() { return Math::OBB{GetPosition(), GetMatrix(), m_HalfSize}; }

		[[nodiscard]] std::array<Vec3, 8> GetVertices() const;
	};
} // namespace Voxymore::Core
