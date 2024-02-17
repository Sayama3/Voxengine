//
// Created by ianpo on 16/02/2024.
//

#pragma once

#include <utility>

#include "Components.hpp"
#include "CustomComponent.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Renderer/Light.hpp"

namespace Voxymore::Core
{
	class LightComponent : public Component<LightComponent>
	{
		VXM_IMPLEMENT_COMPONENT(LightComponent);
	public:
		inline LightComponent() = default;
		inline ~LightComponent() = default;
	public:
		void DeserializeComponent(YAML::Node& node);
		void SerializeComponent(YAML::Emitter& out);
		bool OnImGuiRender();

		Light AsLight(const TransformComponent& tc) const;
		Light AsLight(const glm::vec3& position, const glm::vec3& direction) const;

	private:
		glm::vec4 m_Color = {1,1,1,1};
		float m_Intensity = 1;
		float m_Range = 10;
		LightType m_LightType;
	};

	VXM_CREATE_COMPONENT(LightComponent)

} // namespace Voxymore::Core

