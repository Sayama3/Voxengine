//
// Created by ianpo on 12/04/2024.
//

#include "Voxymore/Editor/Panels/AssetManagerPanel.hpp"
#include "Voxymore/Assets/EditorAssetManager.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Assets/Asset.hpp"

using namespace Voxymore::Core;
namespace fs = std::filesystem;

namespace Voxymore::Editor
{
	AssetManagerPanel::AssetManagerPanel()
	{
		VXM_PROFILE_FUNCTION();
		Ref<EditorAssetManager> assetManager = Project::GetActive()->GetEditorAssetManager();
		if(s_BackTexture == nullptr) {
			s_BackTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/FileSystem/folder-up-256.png"});
		}
		if(s_FolderTexture == nullptr) {
			s_FolderTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/FileSystem/folder-256.png"});
		}
		if(s_FullFolderTexture == nullptr) {
			s_FullFolderTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/FileSystem/folder-full-256.png"});
		}
		if(s_EmptyFolderTexture == nullptr) {
			s_EmptyFolderTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/FileSystem/folder-empty-256.png"});
		}
		if(s_FileTexture == nullptr) {
			s_FileTexture = TextureImporter::LoadTexture2D(Path{FileSource::EditorAsset, "Images/FileSystem/file-256.png"});
		}
	}

	void AssetManagerPanel::OnImGuiRender()
	{
		Ref<EditorAssetManager> assetManager = Project::GetActive()->GetEditorAssetManager();
	}

	AssetManagerPanel::ButtonClickState AssetManagerPanel::ImageButton(uint64_t id, float size)
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
// namespace Voxymore::Core