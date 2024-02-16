//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/ParticlesPhysics/Particle.hpp"
#include "Voxymore/Scene/Entity.hpp"

namespace Voxymore::Core
{
	class ParticlePhysicsLayer;

	class ParticleComponent : public Particle, public Component
	{
		friend class ParticlePhysicsLayer;
		VXM_IMPLEMENT_COMPONENT(ParticleComponent);
	public:
		virtual void DeserializeComponent(YAML::Node& node) override;
		virtual void SerializeComponent(YAML::Emitter& out) override;
		virtual bool OnImGuiRender() override;
		ParticleComponent();
		~ParticleComponent();
		ParticleComponent(Vec3 acceleration, Vec3 velocity, Real damping, Real mass);
	};

} // namespace Voxymore::Core

