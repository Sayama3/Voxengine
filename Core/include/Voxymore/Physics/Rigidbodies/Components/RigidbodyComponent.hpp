//
// Created by Iannis on 12/02/2024.
//

#pragma once

#include "Voxymore/Physics/Rigidbodies/Rigidbody.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.hpp"


namespace Voxymore::Core
{

	class RigidbodyComponent : public Rigidbody, public Component
	{
		VXM_IMPLEMENT_COMPONENT(RigidbodyComponent);
	public:
		virtual void DeserializeComponent(YAML::Node& node) override;
		virtual void SerializeComponent(YAML::Emitter& out) override;
		virtual bool OnImGuiRender() override;

		inline RigidbodyComponent() = default;
		inline ~RigidbodyComponent() = default;
		RigidbodyComponent(Real inverseMass, Real linearDamping, Vec3 position, Quat orientation, Mat3 inverseInertiaTensor);
	};

} // namespace Voxymore::Core


