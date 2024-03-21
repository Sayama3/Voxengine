//
// Created by Iannis on 12/02/2024.
//

#pragma once

#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/RigidbodiesPhysics/Rigidbody.hpp"
#include "Voxymore/Scene/Entity.hpp"


namespace Voxymore::Core
{

	class RigidbodyComponent : public Rigidbody, public SelfAwareComponent<RigidbodyComponent>
	{
		VXM_IMPLEMENT_COMPONENT(RigidbodyComponent);
	public:
		void DeserializeComponent(YAML::Node& node, Entity entity);
		void SerializeComponent(YAML::Emitter& out, Entity entity);
		bool OnImGuiRender(Entity entity);

		inline RigidbodyComponent() = default;
		inline ~RigidbodyComponent() = default;
		RigidbodyComponent(Real inverseMass, Real linearDamping, TransformComponent* transform, Mat3 inverseInertiaTensor);
	};

	struct DisableRigidbody {};

} // namespace Voxymore::Core


