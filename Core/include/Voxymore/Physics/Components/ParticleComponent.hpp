//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Physics/Particle.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.hpp"

namespace Voxymore::Core
{
	class PhysicsLayer;

	class ParticleComponent : public Particle, public Component<ParticleComponent>
	{
		friend class PhysicsLayer;
		VXM_IMPLEMENT_COMPONENT(ParticleComponent);
	public:
		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();
		ParticleComponent();
		~ParticleComponent();
		ParticleComponent(Vec3 acceleration, Vec3 velocity, Real damping, Real mass);
	};

} // namespace Voxymore::Core

