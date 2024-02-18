//
// Created by Iannis on 12/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/RigidbodiesPhysics/Components/RigidbodyComponent.hpp"
#include "Voxymore/Scene/Systems.hpp"
#include "static_block.hpp"

namespace Voxymore::Core
{
	class RigidbodyGravitySystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(RigidbodyGravitySystem);
	public:
		RigidbodyGravitySystem() = default;
		~RigidbodyGravitySystem() = default;
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

		Vec3 m_Gravity = Math::Gravity;
	};
} // namespace Voxymore::Core


