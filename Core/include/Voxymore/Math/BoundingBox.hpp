//
// Created by Sayama on 04/03/2024.
//

#pragma once

#include "Voxymore/Math/BoundingObject.hpp"
#include "Voxymore/Math/Math.hpp"

namespace Voxymore::Core
{
	struct BoundingBox : public BoundingObject<BoundingBox>
	{
	public:
		inline BoundingBox() = default;
		inline ~BoundingBox() = default;
		inline BoundingBox(const BoundingBox& one) = default;

		BoundingBox(const Vec3& min, const Vec3& max);
		BoundingBox(const BoundingBox& one, const BoundingBox& two);
	public:
		[[nodiscard]] bool Overlaps(const BoundingBox& other) const;
		[[nodiscard]] Real GetSize() const;
		[[nodiscard]] Real GetGrowth(BoundingBox other) const;
		void Grow(const BoundingBox& other);

		void Move(const Vec3& movement);

		Vec3 GetCenter() const;
		Vec3 GetHalfSize() const;
		Vec3 GetMin() const;
		Vec3 GetMax() const;

		void SetCenter(Vec3 center);
		void SetSize(Vec3 Size);
		void SetHalfSize(Vec3 halfSize);
		void SetMin(Vec3 min);
		void SetMax(Vec3 max);
	private:
		Vec3 m_Min = {0, 0, 0};
		Vec3 m_Max = {0, 0, 0};
	};

} // namespace Voxymore::Core

