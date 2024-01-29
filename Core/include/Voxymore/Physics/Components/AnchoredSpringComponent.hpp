//
// Created by ianpo on 06/01/2024.
//

#pragma once

#include "static_block.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Components/Components.hpp"
#include "Voxymore/Components/CustomComponent.hpp"
#include "Voxymore/Scene/Entity.hpp"

namespace Voxymore::Core
{
	class AnchoredSpringComponent : public Component
	{
		VXM_IMPLEMENT_COMPONENT(AnchoredSpringComponent);
	public:
		AnchoredSpringComponent() = default;
		~AnchoredSpringComponent() = default;

		void DeserializeComponent(YAML::Node& node) override;
		void SerializeComponent(YAML::Emitter& out) override;
		bool OnImGuiRender() override;

		Real SpringConstant = 1.0;
		Real RestLength = 2.0;
		std::vector<EntityField> EntitiesConnected;
	private:
		std::vector<std::string> EntitiesNameHelper;
	};

	static_block{ AnchoredSpringComponent asc; };
}
