//
// Created by Sayama on 04/03/2024.
//

#include "Voxymore/Math/BoundingBox.hpp"

namespace Voxymore::Core
{
	BoundingBox::BoundingBox(const Vec3 &min, const Vec3 &max) : m_Max(max), m_Min(min) { }

	BoundingBox::BoundingBox(const BoundingBox &one, const BoundingBox &two) : m_Min(one.m_Min), m_Max(one.m_Max)
	{
		Grow(two);
	}

	bool BoundingBox::Overlaps(const BoundingBox& other) const
	{
		// Checks if this box is to the right of the other box.
		if(this->m_Min.x > other.m_Max.x)
			return false;
		// Checks if this box is to the left of the other box.
		if(this->m_Max.x < other.m_Min.x)
			return false;
		// Checks if this box is above the other box.
		if(this->m_Min.y > other.m_Max.y)
			return false;
		// Checks if this box is below the other box.
		if(this->m_Max.y < other.m_Min.y)
			return false;
		// Checks if this box is in front of the other box
		if(this->m_Min.z > other.m_Max.z)
			return false;
		// Checks if this box is behind the other box
		if(this->m_Max.z < other.m_Min.z)
			return false;

		// If none of the sides from this and other box are overlapping, return true.
		return true;
	}

	Real BoundingBox::GetSize() const
	{
		auto size = m_Max - m_Min;
		return Math::SqrMagnitude(size);
	}

	Real BoundingBox::GetGrowth(BoundingBox other) const
	{
		other.Grow(*this);
		return other.GetSize();
	}

	void BoundingBox::Grow(const BoundingBox& other)
	{
		m_Min = glm::min(m_Min, other.m_Min);
		m_Max = glm::max(m_Max, other.m_Max);
	}

	void BoundingBox::Move(const Vec3 &movement)
	{
		m_Min += movement;
		m_Max += movement;
	}

	Vec3 BoundingBox::GetCenter() const
	{
		return (m_Max + m_Min) * 0.5f;
	}

	Vec3 BoundingBox::GetHalfSize() const
	{
		return (m_Max - m_Min) * 0.5f;
	}
	Vec3 BoundingBox::GetMin() const
	{
		return m_Min;
	}
	Vec3 BoundingBox::GetMax() const
	{
		return m_Max;
	}


	void BoundingBox::SetCenter(Vec3 center)
	{
		auto movement = center - GetCenter();
		Move(movement);
	}
	void BoundingBox::SetSize(Vec3 size)
	{
		SetHalfSize(size * 0.5f);
	}
	void BoundingBox::SetHalfSize(Vec3 halfSize)
	{
		auto center = GetCenter();
		m_Min = center - halfSize;
		m_Max = center + halfSize;
	}
	void BoundingBox::SetMin(Vec3 min)
	{
		m_Min = min;
	}
	void BoundingBox::SetMax(Vec3 max)
	{
		m_Max = max;
	}
}// namespace Voxymore::Core