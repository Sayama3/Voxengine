//
// Created by Iannis on 12/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Scene/Systems.hpp"
#include "Voxymore/Physics/Rigidbodies/Components/RigidbodyComponent.hpp"
#include "static_block.hpp"

#ifndef VXM_GRAVITY
#define VXM_GRAVITY {0, -9.81, 0}
#endif

namespace Voxymore::Core
{
	class RBGravitySystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(RBGravitySystem);
	public:
		RBGravitySystem() = default;
		~RBGravitySystem() = default;
	protected:
		virtual void DeserializeSystem(YAML::Node& node) override;
		virtual void SerializeSystem(YAML::Emitter& out) override;
		virtual void ResetSystem() override;
	public:
		virtual bool OnImGuiRender() override;
		virtual void Update(Scene& scene, TimeStep ts) override;

		inline virtual bool RunOnAllScenes() override { return true; }
	private:
		void UpdateParticle(entt::entity e, RigidbodyComponent& pc);

		Vec3 m_Gravity = VXM_GRAVITY;
	};

	static_block{ RBGravitySystem::CreateSystem(); };
} // namespace Voxymore::Core


