//
// Created by ianpo on 08/10/2023.
//

#include "Voxymore/Components/ModelComponent.hpp"
#include "Voxymore/Assets/Assets.hpp"
#include <cmath>
#include <cstring>

// ======== ModelComponent ========
namespace Voxymore::Core
{
	void ModelComponent::DeserializeComponent(YAML::Node &componentNode)
	{
		VXM_PROFILE_FUNCTION();
		auto &model = *this;
		model.m_ModelPath = componentNode["Path"].as<Path>();
		model.m_ShaderName = componentNode["ShaderName"].as<std::string>();
	}

	void ModelComponent::SerializeComponent(YAML::Emitter &emitter)
	{
		VXM_PROFILE_FUNCTION();
		auto& model = *this;
		emitter << KEYVAL("Path", model.m_ModelPath);
		emitter << KEYVAL("ShaderName", model.m_ShaderName);
	}

	bool ModelComponent::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		bool componentChanged = false;
		auto& model = *this;
		auto& p = model.m_ModelPath;

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
					componentChanged |= true;
					currentSourceIndex = i;
					p.source = static_cast<FileSource>(i);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		//TODO: use the cpp style ImGui InputText
		{
			const std::string pathStr = p.path.string();
			const int BufferSize = 1024;
			char path[BufferSize];
			int fillSize = std::min(static_cast<int>(pathStr.size()), (BufferSize - 1));
			std::memcpy(path, pathStr.c_str(), fillSize);
			std::memset(path + fillSize, 0, sizeof(path) - fillSize);
			if (ImGui::InputText("Path", path, BufferSize)) {
				componentChanged |= true;
				p.path = path;
			}
		}

		//TODO: use the cpp style ImGui InputText
		{
			std::string& pathStr = model.m_ShaderName;
			const int BufferSize = 1024;
			char shaderName[BufferSize];
			int fillSize = std::min(static_cast<int>(pathStr.size()), (BufferSize - 1));
			std::memcpy(shaderName, pathStr.c_str(), fillSize);
			std::memset(shaderName + fillSize, 0, sizeof(shaderName) - fillSize);
			if (ImGui::InputText("Shader Name", shaderName, BufferSize)) {
				componentChanged |= true;
				pathStr = shaderName;
			}
		}
		return componentChanged;
	}

	void ModelComponent::LoadModel()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(!m_ModelPath.empty() && std::filesystem::exists(m_ModelPath.GetFullPath()), "The path '{0}' is not valid.", m_ModelPath.GetFullPath().string());
		VXM_CORE_ASSERT(ShaderLibrary::GetInstance().Exists(m_ShaderName), "The shader '{0}' doesn't exist.", m_ShaderName);

		m_Model = Assets::GetModel(m_ModelPath);
		m_IsLoaded = m_Model != nullptr;

		if(m_IsLoaded) {
			Ref<Shader> shader = ShaderLibrary::GetInstance().Get(m_ShaderName);
			m_Model->SetShader(shader);
		}
	}

	bool ModelComponent::IsLoaded() const
	{
		VXM_PROFILE_FUNCTION();
		return m_IsLoaded;
	}

	bool ModelComponent::ShouldLoad() const
	{
		VXM_PROFILE_FUNCTION();
		//TODO: use dirty flag ? (~0.3ms to use equivalent)
		return IsValid() &&
			   (
				   !m_IsLoaded
				   || ((!m_ModelPath.equivalent(m_Model->GetPath())) || m_ShaderName != m_Model->GetShader()->GetName())
			   );
	}

	bool ModelComponent::IsValid() const
	{
		VXM_PROFILE_FUNCTION();
		return !m_ModelPath.empty()
			   && FileSystem::Exist(m_ModelPath)
			   && ShaderLibrary::GetInstance().Exists(m_ShaderName);
	}

	//TODO: implement on ModelComponent change (i.e. change model when user choose to change the model)
}
