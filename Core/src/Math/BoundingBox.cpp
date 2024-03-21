//
// Created by Sayama on 04/03/2024.
//

#include "Voxymore/Math/BoundingBox.hpp"

namespace Voxymore::Core
{
	BoundingBox::BoundingBox(const Vec3 &min, const Vec3 &max) : m_Max(max), m_Min(min) { }

	BoundingBox::BoundingBox(const BoundingBox &one, const BoundingBox &two) : m_Min(one.m_Min), m_Max(one.m_Max)
	{
		VXM_PROFILE_FUNCTION();
		Grow(two);
	}

	bool BoundingBox::Overlaps(const BoundingBox& other) const
	{
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
		auto size = m_Max - m_Min;
		return Math::SqrMagnitude(size);
	}

	Real BoundingBox::GetGrowth(BoundingBox other) const
	{
		VXM_PROFILE_FUNCTION();
		other.Grow(*this);
		return other.GetSize();
	}

	void BoundingBox::Grow(const BoundingBox& other)
	{
		VXM_PROFILE_FUNCTION();
		m_Min = glm::min(m_Min, other.m_Min);
		m_Max = glm::max(m_Max, other.m_Max);
	}

	void BoundingBox::Grow(const Vec3 &point)
	{
		VXM_PROFILE_FUNCTION();
		m_Min = glm::min(m_Min, point);
		m_Max = glm::max(m_Max, point);
	}

	void BoundingBox::Move(const Vec3 &movement)
	{
		VXM_PROFILE_FUNCTION();
		m_Min += movement;
		m_Max += movement;
	}

	Vec3 BoundingBox::GetCenter() const
	{
		VXM_PROFILE_FUNCTION();
		return (m_Max + m_Min) * (Real)0.5;
	}

	Vec3 BoundingBox::GetHalfSize() const
	{
		VXM_PROFILE_FUNCTION();
		return (m_Max - m_Min) * (Real)0.5;
	}

	Vec3 BoundingBox::GetMin() const
	{
		return m_Min;
	}

	Vec3 BoundingBox::GetMax() const
	{
		return m_Max;
	}

	bool BoundingBox::IsValid() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Min.x <= m_Max.x && m_Min.y <= m_Max.y && m_Min.z <= m_Max.z;
	}

	void BoundingBox::SetCenter(Vec3 center)
	{
		VXM_PROFILE_FUNCTION();
		auto movement = center - GetCenter();
		Move(movement);
	}

	void BoundingBox::SetSize(Vec3 size)
	{
		VXM_PROFILE_FUNCTION();
		SetHalfSize(size * (Real)0.5);
	}

	void BoundingBox::SetHalfSize(Vec3 halfSize)
	{
		VXM_PROFILE_FUNCTION();
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

	void BoundingBox::SetMinMax(Vec3 min, Vec3 max)
	{
		m_Min = min;
		m_Max = max;
	}

	void BoundingBox::SetCenterSize(Vec3 center, Vec3 size)
	{
		VXM_PROFILE_FUNCTION();
		auto half = size * (Real)0.5;
		m_Min = center - half;
		m_Max = center + half;
	}

	BoundingBox::operator bool() const
	{
		VXM_PROFILE_FUNCTION();
		return IsValid();
	}
}// namespace Voxymore::Core