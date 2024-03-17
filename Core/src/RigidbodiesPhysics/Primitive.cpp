//
// Created by Sayama on 06/03/2024.
//

#include "Voxymore/RigidbodiesPhysics/Primitive.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"


namespace Voxymore::Core
{
	Vec3 PrimitiveCollider::GetPosition() const
	{
		return m_Offset[3];
	}

	Vec3 PrimitiveCollider::GetAxis(int32_t i) const {
		VXM_CORE_ASSERT(i >= 0, "The value i ({0}) is negative", i);
		VXM_CORE_ASSERT(i < m_Offset.length(), "The value i ({0}) is not valid (superior to {1})", i, m_Offset.length());
		return m_Offset[i];
	}

	std::array<Vec3, 8> Box::GetVertices() const
	{
		VXM_PROFILE_FUNCTION();
		std::array<Vec3, 8> positions = {
			Vec3{-m_HalfSize.x, -m_HalfSize.y, -m_HalfSize.z},
			Vec3{-m_HalfSize.x, -m_HalfSize.y, +m_HalfSize.z},
			Vec3{-m_HalfSize.x, +m_HalfSize.y, -m_HalfSize.z},
			Vec3{-m_HalfSize.x, +m_HalfSize.y, +m_HalfSize.z},
			Vec3{+m_HalfSize.x, -m_HalfSize.y, -m_HalfSize.z},
			Vec3{+m_HalfSize.x, -m_HalfSize.y, +m_HalfSize.z},
			Vec3{+m_HalfSize.x, +m_HalfSize.y, -m_HalfSize.z},
			Vec3{+m_HalfSize.x, +m_HalfSize.y, +m_HalfSize.z},
		};

		for (auto& position : positions) {
			auto pos = m_Offset * Vec4(position, 1);
			position = pos / pos.w;
		}

		return positions;
	}
} // namespace Voxymore::Core
