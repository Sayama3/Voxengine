//
// Created by ianpo on 26/02/2024.
//

#pragma once

#include "Voxymore/Math/Math.hpp"

namespace Voxymore::Core
{
	template<typename BoundingClass>
	class BoundingObject
	{
	public:
		bool Overlaps(const BoundingObject<BoundingClass>& other) const;
		Real GetSize() const;
		void Grow(const BoundingObject<BoundingClass>& other);
	};

	template<typename BoundingClass>
	bool BoundingObject<BoundingClass>::Overlaps(const BoundingObject<BoundingClass> &other) const
	{
		VXM_PROFILE_FUNCTION();
		return BoundingClass::Overlaps(other);
	}

	template<typename BoundingClass>
	Real BoundingObject<BoundingClass>::GetSize() const
	{
		VXM_PROFILE_FUNCTION();
		return BoundingClass::GetSize();
	}

	template<typename BoundingClass>
	void BoundingObject<BoundingClass>::Grow(const BoundingObject<BoundingClass> &other)
	{
		VXM_PROFILE_FUNCTION();
		BoundingClass::Grow(other);
	}
} // namespace Voxymore::Core

