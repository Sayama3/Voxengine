//
// Created by Sayama on 04/03/2024.
//

#pragma once

#include "Voxymore/Math/BoundingObject.hpp"
#include "Voxymore/Math/Math.hpp"
#include <vector>

namespace Voxymore::Core
{
	struct BoundingBox : public BoundingObject<BoundingBox>
	{
	public:
		inline BoundingBox() = default;
		inline ~BoundingBox() = default;
		inline BoundingBox(const BoundingBox& one) = default;

		BoundingBox(const std::vector<Vec3>& points);
		BoundingBox(const std::initializer_list<Vec3>& points);
		BoundingBox(const Vec3& min, const Vec3& max);
		BoundingBox(const BoundingBox& one, const BoundingBox& two);
	public:
		[[nodiscard]] bool Overlaps(const BoundingBox& other) const;
		[[nodiscard]] Real GetSize() const;
		[[nodiscard]] Real GetGrowth(BoundingBox other) const;
		void Grow(const BoundingBox& other);
		void Grow(const Vec3& point);

		void Move(const Vec3& movement);

		[[nodiscard]] Vec3 GetCenter() const;
		[[nodiscard]] Vec3 GetHalfSize() const;
		[[nodiscard]] Vec3 GetMin() const;
		[[nodiscard]] Vec3 GetMax() const;
		[[nodiscard]] bool IsValid() const;

		void SetCenter(Vec3 center);
		void SetSize(Vec3 Size);
		void SetHalfSize(Vec3 halfSize);
		void SetMin(Vec3 min);
		void SetMax(Vec3 max);

		void SetMinMax(Vec3 min, Vec3 max);
		void SetCenterSize(Vec3 center, Vec3 size);

		[[nodiscard]] operator bool() const;
	private:
		Vec3 m_Min = Vec3(std::numeric_limits<Real>::max());
		Vec3 m_Max = Vec3(std::numeric_limits<Real>::min());
	};

} // namespace Voxymore::Core

