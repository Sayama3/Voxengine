//
// Created by Sayama on 06/03/2024.
//

#include "Voxymore/RigidbodiesPhysics/Primitive.hpp"


namespace Voxymore::Core
{
	Vec3 Primitive::GetPosition() const
	{
		VXM_PROFILE_FUNCTION();
		Vec4 position = m_Offset * Vec4(m_Body->GetPosition(),1);
		return position / position.w;
	}
} // namespace Voxymore::Core
