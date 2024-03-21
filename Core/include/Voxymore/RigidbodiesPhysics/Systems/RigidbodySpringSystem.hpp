//
// Created by ianpo on 18/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/Scene/Systems.hpp"
#include "static_block.hpp"

namespace Voxymore::Core
{

	class RigidbodySpringSystem : public System
	{
	VXM_IMPLEMENT_SYSTEM(RigidbodySpringSystem);
	public:
		RigidbodySpringSystem() = default;
		~RigidbodySpringSystem() = default;
	public:
		virtual void Update(Scene& scene, TimeStep ts) override;
	};

} // namespace Voxymore::Core

