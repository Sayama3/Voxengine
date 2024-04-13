//
// Created by ianpo on 07/04/2024.
//

#include "Voxymore/Editor/Panels/ContentBrowserPanel.hpp"
#include "Voxymore/Editor/Panels/PropertyPanel.hpp"
#include "Voxymore/Assets/EditorAssetManager.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Assets/Importers/MaterialSerializer.hpp"
#include "Voxymore/Assets/Importers/MeshImporter.hpp"
#include "Voxymore/Assets/Importers/SceneImporter.hpp"
#include "Voxymore/Assets/Importers/ShaderSerializer.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include <imgui_internal.h>

using namespace Voxymore::Core;
namespace fs = std::filesystem;

namespace Voxymore::Editor
{
	static const std::vector<std::string> s_SourceNames = GetFileSourceNames();
	ContentBrowserPanel::ContentBrowserPanel() : m_Path({FileSource::None, ""}), m_ThumbnailSize(s_ThumbnailSize), m_Padding(s_Padding)
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

	ContentBrowserPanel::ContentBrowserPanel(Core::Path p) : m_Path(p), m_ThumbnailSize(s_ThumbnailSize), m_Padding(s_Padding)
	{
		Ref<EditorAssetManager> assetManager = Project::GetActive()->GetEditorAssetManager();
		VXM_PROFILE_FUNCTION();
		if(s_BackTexture == nullptr) {
			s_BackTexture = CastPtr<Texture2D>(assetManager->GetOrCreateAsset({FileSource::EditorAsset, "Images/FileSystem/folder-up-256.png"}));
		}
		if(s_FolderTexture == nullptr) {
			s_FolderTexture = CastPtr<Texture2D>(assetManager->GetOrCreateAsset({FileSource::EditorAsset, "Images/FileSystem/folder-256.png"}));
		}
		if(s_FullFolderTexture == nullptr) {
			s_FullFolderTexture = CastPtr<Texture2D>(assetManager->GetOrCreateAsset({FileSource::EditorAsset, "Images/FileSystem/folder-full-256.png"}));
		}
		if(s_EmptyFolderTexture == nullptr) {
			s_EmptyFolderTexture = CastPtr<Texture2D>(assetManager->GetOrCreateAsset({FileSource::EditorAsset, "Images/FileSystem/folder-empty-256.png"}));
		}
		if(s_FileTexture == nullptr) {
			s_FileTexture = CastPtr<Texture2D>(assetManager->GetOrCreateAsset({FileSource::EditorAsset, "Images/FileSystem/file-256.png"}));
		}
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		VXM_PROFILE_FUNCTION();

		ImGui::Text("Zoom :  ");
		ImGui::SameLine();
		if(ImGui::Button(" - ")){
			s_ThumbnailSize = m_ThumbnailSize = Math::Clamp(m_ThumbnailSize - c_Increment, c_ThumbnailSizeMin, c_ThumbnailSizeMax) ;
		}
		ImGui::SameLine();
		if(ImGui::Button(" + ")) {
			s_ThumbnailSize = m_ThumbnailSize = Math::Clamp(m_ThumbnailSize + c_Increment, c_ThumbnailSizeMin, c_ThumbnailSizeMax) ;
		}
		ImGui::SameLine();
		if(ImGui::SliderFloat("##Icon Size", &m_ThumbnailSize, c_ThumbnailSizeMin, c_ThumbnailSizeMax)) {
			s_ThumbnailSize = m_ThumbnailSize;
		}

		/*
		if(ImGui::SliderFloat("Padding", &m_Padding, 0, 32)) {
			s_Padding = m_Padding;
		}
		 */

		float ratio = (m_ThumbnailSize - c_ThumbnailSizeMin) / (c_ThumbnailSizeMax - c_ThumbnailSizeMin);
		float originalFontScale = ImGuiLib::GetWindowFontScale();
		ImGui::SetWindowFontScale(ratio*.4f + 0.8f);



		if(m_Path.source == FileSource::None) {
			DrawRoot();
		} else if(FileSystem::Exist(m_Path)) {
			DrawContent();
		} else {
			m_Path = {FileSource::None, ""};
		}

		ImGui::SetWindowFontScale(originalFontScale);

		if(ImGui::IsWindowHovered(ImGuiHoveredFlags_None) && !ImGui::IsAnyItemHovered()) {
			if (m_Path.source == Core::FileSource::Asset && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				ImGui::OpenPopup("ContentBrowserContextMenu");
			}
			else if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				PropertyPanel::Reset();
			}
		}

		if (ImGui::BeginPopup("ContentBrowserContextMenu")) {
			bool isAsset = m_Path.source == Core::FileSource::Asset;
			if(isAsset) {
				auto assetManager = Project::GetActive()->GetEditorAssetManager();
				if(ImGui::MenuItem("New Folder")) {
					static uint64_t folderCounter = 0;
					auto newFolder = m_Path;
					newFolder.path /= "NewFolder_" + std::to_string(folderCounter++);
					if(!FileSystem::Exist(newFolder)) {
						std::filesystem::create_directory(newFolder);
					} else {
						VXM_CORE_ERROR("A folder named 'New Folder' already exist here.");
					}
					ImGui::CloseCurrentPopup();
				}

				if(ImGui::MenuItem("New Shader")) {
					static uint64_t shaderCounter = 0;
					auto newShaderPath = m_Path;
					newShaderPath.path /= "NewShader_" + std::to_string(shaderCounter++) + ShaderExtension;
					Ref<Shader> shader = Shader::Create(newShaderPath.path.stem().string(), std::unordered_map<ShaderType, ShaderSourceField>{});
					if(assetManager->AddAsset(shader, newShaderPath)) {
						ShaderSerializer::ExportEditorShader(assetManager->GetMetadata(shader->Handle), shader);
					}
					ImGui::CloseCurrentPopup();
				}

				if(ImGui::MenuItem("New Material")) {
					static uint64_t materialCount = 0;
					auto newMaterialPath = m_Path;
					newMaterialPath.path /= "NewMaterial_" + std::to_string(materialCount++) + MaterialExtension;
					Ref<Material> material = assetManager->CreateAsset<Material>(newMaterialPath, newMaterialPath.path.stem().string());
					if(material) {
						MaterialSerializer::ExportEditorMaterial(assetManager->GetMetadata(material->Handle), material);
					}
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
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
				if (filename.ends_with(".meta")) {
					continue;
				}

				Path assetPath = m_Path;
				assetPath.path /= filename;

				ImGui::PushID(pathStr.c_str());

				bool isDirectory = entry.is_directory();
				bool isFile = !isDirectory && (fs::is_regular_file(assetPath) || fs::is_character_file(assetPath));
				bool isInData = m_Path.source == Core::FileSource::Asset;
				Ref<Texture2D> image = isDirectory ? (std::filesystem::is_empty(path) ? s_EmptyFolderTexture : s_FullFolderTexture) : s_FileTexture;

				ButtonClickState state = ImageButton(image->GetRendererID(), m_ThumbnailSize);
				if (state == STATE_DOUBLE_CLICKED) {
					if (isDirectory) {
						m_Path = assetPath;
					}
					else if (isFile) {
						auto metadata = Project::GetActive()->GetEditorAssetManager()->GetMetadata(assetPath);
						if (metadata) {
							PropertyPanel::SetSelectedAsset(AssetManager::GetAsset(metadata.Handle));
						}
					}
				}
				else if (state == STATE_HOVERED && isFile) {
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
						ImGui::OpenPopup(pathStr.c_str());
					}
					else if (AssetImporter::GetAssetType(assetPath) != AssetType::None) {
						Ref<EditorAssetManager> assetManager = Project::GetActive()->GetEditorAssetManager();
						Ref<Asset> asset = assetManager->GetOrCreateAsset(assetPath);
						if (asset && ImGui::BeginDragDropSource()) {
							AssetMetadata metadata = assetManager->GetMetadata(asset->Handle);
							std::string payloadID = AssetTypeToPayloadID(metadata.Type);
							VXM_CORE_ASSERT(payloadID.size() < 32, "The payloadID is not high.");
							ImGui::SetDragDropPayload(payloadID.c_str(), &metadata.Handle, sizeof(metadata.Handle), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
						else if (!asset) {
							VXM_CORE_ERROR("The asset '{0}' failed to load.", assetPath.string());
						}
					}
				}

				if (ImGui::BeginPopup(pathStr.c_str())) {
					auto assetManager = Project::GetActive()->GetEditorAssetManager();
					auto metadata = assetManager->GetMetadata(assetPath);

					if (ImGui::MenuItem("Delete")) {
						if (metadata) {
							assetManager->RemoveAsset(metadata.Handle);
						}
						std::filesystem::remove(assetPath);
						ImGui::CloseCurrentPopup();
					}
					else if (!metadata && AssetImporter::GetAssetType(assetPath) != AssetType::None && ImGui::MenuItem("Import")) {
						assetManager->ImportAsset(assetPath);
						if (assetManager) {
							VXM_CORE_INFO("Asset '{0}' was successfully imported.", filename);
						}
						else {
							VXM_CORE_ERROR("Asset '{0}' failed to be imported.", filename);
						}
						ImGui::CloseCurrentPopup();
					}
					else if (metadata) {
						if (assetManager->IsAssetLoaded(metadata.Handle)) {
							if (ImGui::MenuItem("Unload")) {
								assetManager->UnloadAsset(metadata.Handle);
								ImGui::CloseCurrentPopup();
							}
						}
						else {
							if (ImGui::MenuItem("Load")) {
								assetManager->GetAsset(metadata.Handle);
								ImGui::CloseCurrentPopup();
							}
						}
					}
					ImGui::EndPopup();
				}

				if (isFile && isInData)
				{
					std::array<char, 255> tmpFileName{};
					std::fill(tmpFileName.begin(), tmpFileName.end(), (char) 0);
					std::memcpy(tmpFileName.data(), filename.c_str(), std::min(filename.size(), tmpFileName.size() - 1));

					if (ImGui::InputText("##FileName", tmpFileName.data(), tmpFileName.size(), ImGuiInputTextFlags_AlwaysOverwrite)) {
							filename = tmpFileName.data();
							auto fullAssetPath = assetPath.GetFullPath();
							auto newPath = fullAssetPath.parent_path() / filename;
							fs::rename(fullAssetPath, newPath);

					}
//					if(ImGui::)
				}
				else
				{
					ImGui::TextWrapped("%s", filename.c_str());
				}

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