//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Physics/Particle.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.hpp"

namespace Voxymore::Core
{
	class ParticleComponent
	{
		VXM_IMPLEMENT_COMPONENT(ParticleComponent);
	public:
		inline ParticleComponent() = default;
		inline ~ParticleComponent() = default;
		inline ParticleComponent(Vec3 acceleration, Vec3 velocity, Real damping, Real mass);

		Vec3 Acceleration = Vec3(0.0);
		Vec3 Velocity = Vec3(0.0);
		Real Damping = 0.9;
		Real InverseMass = 1.0;

		[[nodiscard]] Particle GetParticle(Entity e) const;
		[[nodiscard]] Particle GetParticle(const Vec3& pos) const;
	};

} // namespace Voxymore::Core

