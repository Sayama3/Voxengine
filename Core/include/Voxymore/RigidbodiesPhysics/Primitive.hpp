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
		Rigidbody* m_Body;
		Mat4 m_Offset;

		[[nodiscard]] Vec3 GetPosition() const;
	};

	class Sphere : public Primitive
	{
	public:
		Real m_Radius;
	};

	class Plane : public Primitive
	{
	public:
		Vec3 m_Normal;
		Real m_Offset;
	};

} // namespace Voxymore::Core

