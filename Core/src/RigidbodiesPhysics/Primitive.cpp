//
// Created by Sayama on 06/03/2024.
//

#include "Voxymore/RigidbodiesPhysics/Primitive.hpp"


namespace Voxymore::Core
{
	Vec3 Primitive::GetPosition() const
	{
		VXM_PROFILE_FUNCTION();
		return m_Offset[3];
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
