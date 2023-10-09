#include "Voxymore/Scene/ModelComponent.hpp"
#include <cstring>
#include <cmath>

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
		auto& p = model.m_Path;

		std::vector<std::string> PossibleFileSources = GetFileSourceNames();
		int currentSourceIndex = static_cast<int>(p.source); // Here we store our selection data as an index.
		const std::string&currentSourceName = PossibleFileSources[currentSourceIndex];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Source", currentSourceName.c_str()))
		{
			for (int i = 0; i < PossibleFileSources.size(); i++)
			{
				const bool is_selected = (currentSourceIndex == i);
				if (ImGui::Selectable(PossibleFileSources[i].c_str(), is_selected))
				{
					currentSourceIndex = i;
					p.source = static_cast<FileSource>(i);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		//TODO: use the cpp style ImGui InputText
		const std::string pathStr = p.path.string();
		const int BufferSize = 1024;
		char path[BufferSize];
		int fillSize = std::min(static_cast<int>(pathStr.size()), (BufferSize-1));
		std::memcpy(path, pathStr.c_str(), fillSize);
		std::memset(path+fillSize, 0, sizeof(path)-fillSize);
		if(ImGui::InputText("Path", path, BufferSize))
		{
			p.path = path;
		}
    }

	void ModelComponent::LoadModel()
	{
		VXM_CORE_ASSERT(!m_IsLoaded, "The model is already loaded.");
		VXM_CORE_ASSERT(std::filesystem::exists(m_Path.GetFullPath()), "The path '{0}' is not valid.", m_Path.GetFullPath().string());
		m_Model = Model::CreateModel(m_Path);
		m_IsLoaded = true;
	}
}
