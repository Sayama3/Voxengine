//
// Created by ianpo on 06/01/2024.
//

#pragma once

#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Scene/Entity.hpp"
#include "static_block.hpp"

namespace Voxymore::Core
{
	class AnchoredSpringComponent : public Component<AnchoredSpringComponent>
	{
		VXM_IMPLEMENT_COMPONENT(AnchoredSpringComponent);
	public:
		AnchoredSpringComponent() = default;
		~AnchoredSpringComponent() = default;

		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();

		Real SpringConstant = 1.0;
		Real RestLength = 2.0;
		std::vector<EntityField> EntitiesConnected;
	private:
		std::vector<std::string> EntitiesNameHelper;
	};
}
