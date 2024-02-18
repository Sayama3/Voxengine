//
// Created by ianpo on 16/02/2024.
//

#include "Voxymore/Components/LightComponent.hpp"

namespace Voxymore::Core
{
	void LightComponent::DeserializeComponent(YAML::Node& node)
	{
		m_Color = node["Color"].as<glm::vec4>();
		m_Intensity = node["Intensity"].as<Real>();
		m_Range = node["Range"].as<Real>();
		m_Cutoff = node["Cutoff"].as<Real>();
		m_LightType = (LightType)node["LightType"].as<int>();
	}

	void LightComponent::SerializeComponent(YAML::Emitter& out)
	{
		out << KEYVAL("Color", m_Color);
		out << KEYVAL("Intensity", m_Intensity);
		out << KEYVAL("Range", m_Range);
		out << KEYVAL("Cutoff", m_Cutoff);
		out << KEYVAL("LightType", (int)m_LightType);
	}

	bool LightComponent::OnImGuiRender()
	{
		bool changed = false;
		changed |= ImGui::ColorPicker4("Color", glm::value_ptr(m_Color));
		changed |= ImGui::DragFloat("Intensity", &m_Intensity, 1);
		changed |= ImGui::DragFloat("Range", &m_Range, 0.1);
		changed |= ImGui::DragFloat("Cutoff", &m_Cutoff, 0.1, 0, 360);
		changed |= ImGui::Combo("Light Type", (int *) &m_LightType, "Directional\0Point\0Spot\0");
		return changed;
	}

	Light LightComponent::AsLight(const TransformComponent &tc) const
	{
		return Light(m_Color, tc.GetPosition(), tc.GetForward(), m_Range, m_Intensity, glm::radians(m_Cutoff), m_LightType);
	}

	Light LightComponent::AsLight(const glm::vec3& position, const glm::vec3& direction) const
	{
		return Light(m_Color, position, direction, m_Range, m_Intensity, glm::radians(m_Cutoff), m_LightType);
	}
}// namespace Voxymore::Core