//
// Created by ianpo on 18/02/2024.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.hpp"

namespace Voxymore::Core
{
	struct ConnectedEntity
	{
		EntityField Entity;
		Vec3 BodyPosition;
	};


	class RigidbodySpringComponent : public Component<RigidbodySpringComponent>
	{
		VXM_IMPLEMENT_COMPONENT(RigidbodySpringComponent);
	public:
		RigidbodySpringComponent() = default;
		~RigidbodySpringComponent() = default;

		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();

		std::vector<ConnectedEntity> EntitiesConnected;
	private:
		std::vector<std::string> EntitiesNameHelper;
	public:
		Vec3 LocalPosition = Vec3(0);
		Real SpringConstant = 1.0;
		Real RestLength = 2.0;
	};

} // namespace Voxymore::Core

