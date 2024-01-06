//
// Created by ianpo on 06/01/2024.
//

#pragma once

#include "Voxymore/Physics/Components/AnchoredSpringComponent.hpp"
#include "Voxymore/Scene/Systems.hpp"

namespace Voxymore::Core
{

	class SpringForceSystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(SpringForceSystem);
	public:
		void Update(Scene& scene, TimeStep ts) override;
		inline bool RunOnAllScenes() override {return false;}
	};

} // namespace Voxymore::Core

