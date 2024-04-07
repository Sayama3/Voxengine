//
// Created by ianpo on 07/04/2024.
//

#include "Voxymore/Editor/Panels/ContentBrowserPanel.hpp"

using namespace Voxymore::Core;
namespace fs = std::filesystem;

namespace Voxymore::Editor
{

	void ContentBrowserPanel::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();
		if(m_Path.source == FileSource::None) {
			static std::vector<std::string> sourceNames = GetFileSourceNames();

			ImGui::BeginDisabled(!FileSystem::Exist({FileSource::Asset, ""}));
			if(ImGui::Button(sourceNames[(uint32_t)FileSource::Asset].c_str()))
			{
				m_Path = {FileSource::Asset, ""};
			}
			ImGui::EndDisabled();

			ImGui::BeginDisabled(!FileSystem::Exist({FileSource::EditorAsset, ""}));
			if(ImGui::Button(sourceNames[(uint32_t)FileSource::EditorAsset].c_str()))
			{
				m_Path = {FileSource::EditorAsset, ""};
			}
			ImGui::EndDisabled();
		} else {
			if (ImGui::Button("..")) {
				if (m_Path.path == "") {
					m_Path = {FileSource::None, ""};
				} else {
					m_Path.path = m_Path.path.parent_path();
				}
			} else {
				for (const fs::directory_entry &entry: fs::directory_iterator(m_Path)) {
					const fs::path &path = entry.path();
					std::string pathStr = entry.path().string();
					std::string filename = path.filename().string();

					if(filename.ends_with(".meta")) {
						continue;
					}

					ImGui::BeginDisabled(!entry.is_directory());
					if (ImGui::Button(filename.c_str())) {
						m_Path.path /= filename;
					}
					ImGui::EndDisabled();
				}
			}
		}
	}
}