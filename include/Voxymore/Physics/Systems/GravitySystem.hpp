//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Physics/Components/ParticleComponent.hpp"
#include "Voxymore/Scene/Systems.hpp"

namespace Voxymore::Core
{

	class GravitySystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(GravitySystem);
	protected:
		virtual void DeserializeSystem(YAML::Node& node) override;
		virtual void SerializeSystem(YAML::Emitter& out) override;
		virtual void ResetSystem() override;
	public:
		virtual bool OnImGuiRender() override;
		virtual void Update(Scene& scene, TimeStep ts) override;

		inline virtual bool RunOnAllScenes() override { return true; }
	private:
		void UpdateParticle(entt::entity e, ParticleComponent& pc);
		Vec3 m_Gravity = Vec3(0.0, -9.81, 0.0);
	};

} // namespace Voxymore::Core

