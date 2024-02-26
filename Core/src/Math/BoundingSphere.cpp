//
// Created by ianpo on 26/02/2024.
//

#include "Voxymore/Math/BoundingSphere.hpp"
#include "Voxymore/Core/Core.hpp"

namespace Voxymore::Core
{
	bool BoundingSphere::Overlaps(const BoundingSphere& other) const
	{
		VXM_PROFILE_FUNCTION();
		// Calculate the squared distance between the two centers
		Real distSquared = Math::SqrMagnitude(this->m_Center - other.m_Center);

		// Calculate the squared sum of the radius
		Real radiusSumSquared = (this->m_Radius + other.m_Radius) * (this->m_Radius + other.m_Radius);

		// Return true if the squared distance is less than the squared sum of their radius
		return distSquared < radiusSumSquared;   // The spheres do not overlap
	}

	Real BoundingSphere::GetSize() const {
		VXM_PROFILE_FUNCTION();
		return m_Radius;
	}
	BoundingSphere::BoundingSphere(const Vec3& center, Real radius) : m_Center(center), m_Radius(radius) {}

	BoundingSphere::BoundingSphere(const BoundingSphere &one, const BoundingSphere &two) : m_Center(one.m_Center), m_Radius(one.m_Radius)
	{
		VXM_PROFILE_FUNCTION();
		Grow(two);
	}

	void BoundingSphere::Grow(const BoundingSphere &other)
	{
		VXM_PROFILE_FUNCTION();
		Vec3 centerOffset = other.m_Center - m_Center;
		Real distance = Math::SqrMagnitude(centerOffset);
		Real radiusDiff = other.m_Radius - m_Radius;

		// No need to Sqrt yet so we do a multiplication.
		if(radiusDiff * radiusDiff >= distance)
		{
			if(m_Radius < other.m_Radius)
			{
				m_Center = other.m_Center;
				m_Radius = other.m_Radius;
			}
		}
		else
		{
			distance = Math::Sqrt(distance);
			auto oldRadius = m_Radius;
			m_Radius = (m_Radius + other.m_Radius + distance) * ((Real)0.5);
			if(distance > 0)
			{
				m_Center += (centerOffset * ((m_Radius - oldRadius) / distance));
			}
		}
	}
	Real BoundingSphere::GetGrowth(BoundingSphere other) const
	{
		other.Grow(*this);
		return other.GetSize() - GetSize();
	}
} // namespace Voxymore::Core
