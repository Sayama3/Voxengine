//
// Created by ianpo on 06/01/2024.
//

#pragma once

#include "Voxymore/ParticlePhysic/Components/AnchoredSpringComponent.hpp"
#include "Voxymore/Scene/Systems.hpp"

namespace Voxymore::Core
{
	class SpringForceSystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(SpringForceSystem);
	public:
		SpringForceSystem() = default;
		~SpringForceSystem() = default;
	public:
		void Update(Scene& scene, TimeStep ts) override;
		inline bool RunOnAllScenes() override {return true;}
	};

	static_block{ SpringForceSystem::CreateSystem(); };

} // namespace Voxymore::Core

