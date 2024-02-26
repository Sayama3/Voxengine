//
// Created by ianpo on 26/02/2024.
//

#pragma once

#include "BoundingObject.hpp"
#include "Math.hpp"

namespace Voxymore::Core
{

	struct BoundingSphere : public BoundingObject<BoundingSphere>
	{
	public:
		inline BoundingSphere() = default;
		inline ~BoundingSphere() = default;
		inline BoundingSphere(const BoundingSphere& one) = default;

		BoundingSphere(const Vec3& center, Real radius);
		BoundingSphere(const BoundingSphere& one,const BoundingSphere& two);
	public:
		[[nodiscard]] bool Overlaps(const BoundingSphere& other) const;
		[[nodiscard]] Real GetSize() const;
		void Grow(const BoundingSphere& other);
	private:
		Vec3 m_Center = {0,0,0};
		Real m_Radius = 0;
	};

} // namespace Voxymore::Core

