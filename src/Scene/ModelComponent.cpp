#include "Voxymore/Scene/ModelComponent.hpp"

// ======== ModelComponent ========
namespace Voxymore::Core
{
    VXM_CREATE_COMPONENT(ModelComponent);

    void ModelComponent::DeserializeComponent(YAML::Node &componentNode, Entity targetEntity)
    {
    	auto &model = targetEntity.GetOrAddComponent<ModelComponent>();
    	model.m_Path = componentNode["Path"].as<Path>();
    }

    void ModelComponent::SerializeComponent(YAML::Emitter &emitter, Entity sourceEntity)
    {
    	auto& model = sourceEntity.GetComponent<ModelComponent>();
		emitter << KEYVAL("Path", model.GetPath());
    }

    void ModelComponent::OnImGuiRender(Entity sourceEntity)
    {
    	auto& model = sourceEntity.GetComponent<ModelComponent>();
    	//ImGui::DragFloat("Path", &model.m_Speed);
        //TODO: Add Path to imgui with field then dropdown.
    }

	void ModelComponent::LoadModel()
	{
	}
}
