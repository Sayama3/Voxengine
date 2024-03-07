//
// Created by Sayama on 06/03/2024.
//

#pragma once

#include "Voxymore/Math/Math.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"

namespace Voxymore::Core
{
	class Primitive
	{
	public:
		Rigidbody* m_Body = nullptr;
		Mat4 m_Offset = Math::Identity<Mat4>();

		[[nodiscard]] virtual Vec3 GetPosition() const;
	};

	class Sphere : public Primitive
	{
	public:
		Real m_Radius = 1;
	};

	class Plane : public Primitive
	{
	public:
		Vec3 m_Normal = Vec3(0,1,0);
		Real m_Offset = 0;
	};
} // namespace Voxymore::Core
