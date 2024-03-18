//
// Created by Sayama on 06/03/2024.
//

#include "Voxymore/RigidbodiesPhysics/Primitive.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"


namespace Voxymore::Core
{
	Vec3 PrimitiveCollider::GetPosition() const
	{
		return m_CachedMatrix[3];
	}

	Mat4 PrimitiveCollider::GetMatrix() const
	{
		return m_CachedMatrix;
	}

	Vec3 PrimitiveCollider::GetAxis(int32_t i) const {
		VXM_CORE_ASSERT(i >= 0, "The value i ({0}) is negative", i);
		VXM_CORE_ASSERT(i < m_CachedMatrix.length(), "The value i ({0}) is not valid (superior to {1})", i, m_CachedMatrix.length());
		return m_CachedMatrix[i];
	}

	void PrimitiveCollider::CacheMatrix()
	{
		m_CachedMatrix = m_Transform ? m_Transform->GetTransform() : Math::Identity<Mat4>();
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
			position = Math::TransformPoint(m_CachedMatrix, position);
		}

		return positions;
	}
} // namespace Voxymore::Core
