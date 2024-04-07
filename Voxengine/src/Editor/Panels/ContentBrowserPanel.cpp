//
// Created by ianpo on 07/04/2024.
//

#include "Voxymore/Editor/Panels/ContentBrowserPanel.hpp"

using namespace Voxymore::Core;
namespace fs = std::filesystem;

namespace Voxymore::Editor
{
	static const std::vector<std::string> s_SourceNames = GetFileSourceNames();
	ContentBrowserPanel::ContentBrowserPanel() : m_Path({FileSource::None, ""}), m_ThumbnailSize(s_ThumbnailSize), m_Padding(s_Padding)
	{
		VXM_PROFILE_FUNCTION();
		if(s_BackTexture == nullptr) {
			s_BackTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-up-256.png"});
		}
		if(s_FolderTexture == nullptr) {
			s_FolderTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-256.png"});
		}
		if(s_FullFolderTexture == nullptr) {
			s_FullFolderTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-full-256.png"});
		}
		if(s_EmptyFolderTexture == nullptr) {
			s_EmptyFolderTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-empty-256.png"});
		}
		if(s_FileTexture == nullptr) {
			s_FileTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/file-256.png"});
		}
	}

	ContentBrowserPanel::ContentBrowserPanel(Core::Path p) : m_Path(p), m_ThumbnailSize(s_ThumbnailSize), m_Padding(s_Padding)
	{
		VXM_PROFILE_FUNCTION();
		if(s_BackTexture == nullptr) {
			s_BackTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-up-256.png"});
		}
		if(s_FolderTexture == nullptr) {
			s_FolderTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-256.png"});
		}
		if(s_FullFolderTexture == nullptr) {
			s_FullFolderTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-full-256.png"});
		}
		if(s_EmptyFolderTexture == nullptr) {
			s_EmptyFolderTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/folder-empty-256.png"});
		}
		if(s_FileTexture == nullptr) {
			s_FileTexture = Assets::GetTexture({FileSource::EditorAsset, "Images/FileSystem/file-256.png"});
		}
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();

		ImGui::Text("Zoom :  ");
		ImGui::SameLine();
		if(ImGui::Button(" - ")){
			s_ThumbnailSize = m_ThumbnailSize = Math::Clamp(m_ThumbnailSize - c_Increment, 16.0f, 512.0f) ;
		}
		ImGui::SameLine();
		if(ImGui::Button(" + ")) {
			s_ThumbnailSize = m_ThumbnailSize = Math::Clamp(m_ThumbnailSize + c_Increment, 16.0f, 512.0f) ;
		}
		ImGui::SameLine();
		if(ImGui::SliderFloat("##Icon Size", &m_ThumbnailSize, 16.f, 512.f)) {
			s_ThumbnailSize = m_ThumbnailSize;
		}

		/*
		if(ImGui::SliderFloat("Padding", &m_Padding, 0, 32)) {
			s_Padding = m_Padding;
		}
		 */

		if(m_Path.source == FileSource::None) {
			DrawRoot();
		} else if(FileSystem::Exist(m_Path)) {
			DrawContent();
		} else {
			m_Path = {FileSource::None, ""};
		}
	}


	void ContentBrowserPanel::DrawRoot()
	{
		VXM_PROFILE_FUNCTION();

		float cellSize = m_Padding + m_ThumbnailSize;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = std::max(int(panelWidth / cellSize), 1);

		//TODO: UseTables
		ImGui::Columns(columnCount, 0, false);

		{
			ImGui::BeginDisabled(!FileSystem::Exist(FileSource::Asset));
			if (ImageButton(s_FolderTexture->GetRendererID(), m_ThumbnailSize) == STATE_DOUBLE_CLICKED) {
				m_Path = {FileSource::Asset, ""};
			}
			ImGui::EndDisabled();
			ImGui::TextWrapped("%s", s_SourceNames[(uint32_t) FileSource::Asset].c_str());
			ImGui::NextColumn();
		}

		{
			ImGui::BeginDisabled(!FileSystem::Exist(FileSource::EditorAsset));
			if (ImageButton(s_FolderTexture->GetRendererID(), m_ThumbnailSize) == STATE_DOUBLE_CLICKED) {
				m_Path = {FileSource::EditorAsset, ""};
			}
			ImGui::EndDisabled();
			ImGui::TextWrapped("%s", s_SourceNames[(uint32_t) FileSource::EditorAsset].c_str());
			//TODO: Use Table API
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
	}


	void ContentBrowserPanel::DrawContent()
	{
		VXM_PROFILE_FUNCTION();
		static std::vector<std::string> sourceNames = GetFileSourceNames();
		fs::path p = s_SourceNames[(uint32_t)m_Path.source] / m_Path.path;
		ImGui::Text("Current Directory: '%s'", p.string().c_str());
		ImGui::Spacing();

		float cellSize = m_Padding + m_ThumbnailSize;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = std::max(int(panelWidth / cellSize), 1);

		//TODO: UseTables
		ImGui::Columns(columnCount, 0, false);

		ButtonClickState goBack = ImageButton(s_BackTexture->GetRendererID(), m_ThumbnailSize);
		ImGui::TextWrapped("Back");
		ImGui::NextColumn();
		if (goBack == STATE_DOUBLE_CLICKED) {
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

				ImGui::PushID(pathStr.c_str());

				bool isDirectory = entry.is_directory();
				Ref<Texture2D> image = isDirectory ? (std::filesystem::is_empty(path) ? s_EmptyFolderTexture : s_FullFolderTexture) : s_FileTexture;

				ButtonClickState state = ImageButton(image->GetRendererID(), m_ThumbnailSize);
				if (state == STATE_DOUBLE_CLICKED && isDirectory) {
					m_Path.path /= filename;
				} else if(state == STATE_HOVERED && ImGui::BeginDragDropSource()) {
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr.data(), pathStr.size()+1, ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}

				ImGui::TextWrapped("%s", filename.c_str());
				//TODO: Use Table API
				ImGui::PopID();
				ImGui::NextColumn();
			}
		}
		ImGui::Columns(1);
	}

	ContentBrowserPanel::ButtonClickState ContentBrowserPanel::ImageButton(uint64_t id, float size)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
		ImGui::ImageButton((ImTextureID)id, ImVec2(size, size), ImVec2(0, 1), ImVec2(1,0));
		ImGui::PopStyleColor();

		ButtonClickState result = STATE_NONE;
		if(ImGui::IsItemHovered()) {
			result = STATE_HOVERED;
			if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				result = STATE_DOUBLE_CLICKED;
			}
			else if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				result = STATE_CLICKED;
			}
		}
		return result;
	}
}