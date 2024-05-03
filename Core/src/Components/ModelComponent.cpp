//
// Created by ianpo on 08/10/2023.
//

#include "Voxymore/Assets/AssetManager.hpp"
#include "Voxymore/Components/ModelComponent.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include <cmath>
#include <cstring>

// ======== ModelComponent ========
namespace Voxymore::Core
{
	void ModelComponent::DeserializeComponent(YAML::Node &componentNode)
	{
		VXM_PROFILE_FUNCTION();
		m_Model = componentNode["Model"].as<ModelField>();
	}

	void ModelComponent::SerializeComponent(YAML::Emitter &emitter)
	{
		VXM_PROFILE_FUNCTION();
		emitter << KEYVAL("Model", m_Model);
	}

	bool ModelComponent::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;

		changed |= ImGuiLib::DrawAssetField("Model", &m_Model);


		return changed;
	}

	bool ModelComponent::IsValid() const
	{
		return m_Model.IsValid();
	}

	//TODO: implement on ModelComponent change (i.e. change model when user choose to change the model)
}
