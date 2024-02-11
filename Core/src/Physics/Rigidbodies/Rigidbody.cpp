//
// Created by ianpo on 11/02/2024.
//

#include "Voxymore/Physics/Rigidbodies/Rigidbody.hpp"

namespace Voxymore::Core
{
	Mat4 Rigidbody::CalculateTransformMatrix() const
	{
		return Math::TRS(m_Position, m_Orientation);
	}
} // namespace Voxymore::Core