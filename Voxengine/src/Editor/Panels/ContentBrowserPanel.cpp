//
// Created by ianpo on 07/04/2024.
//

#include "Voxymore/Editor/Panels/ContentBrowserPanel.hpp"
#include "DefaultShaders.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/EditorAssetManager.hpp"
#include "Voxymore/Assets/ImGui/MaterialImGui.hpp"
#include "Voxymore/Assets/ImGui/ShaderImGui.hpp"
#include "Voxymore/Assets/Importers/MaterialSerializer.hpp"
#include "Voxymore/Assets/Importers/MeshImporter.hpp"
#include "Voxymore/Assets/Importers/SceneImporter.hpp"
#include "Voxymore/Assets/Importers/ShaderSerializer.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Editor/Panels/PropertyPanel.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"
#include <cstring>
#include <imgui_internal.h>

using namespace Voxymore::Core;
namespace fs = std::filesystem;

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace Voxymore::Core;

namespace Voxymore::Editor
{
	std::unordered_map<Core::AssetType, ContentBrowserPanel::ImGuiMenuFunc> ContentBrowserPanel::s_MenuFunc = {
			{AssetType::Material, &MaterialImGui::OnMenuMaterialImGui},
			{AssetType::Shader, &ShaderImGui::OnMenuShaderImGui},
	};
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
		VXM_PROFILE_FUNCTION();
		Ref<EditorAssetManager> assetManager = Project::GetActive()->GetEditorAssetManager();
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

		auto assetManager = Project::GetActive()->GetEditorAssetManager();

		enum class PopupToOpen {
			None,
			NewFolder,
			NewShaderSource,
			NewShader,
			NewMaterial,
		};

		PopupToOpen popupToOpen = PopupToOpen::None;

		if (ImGui::BeginPopup("ContentBrowserContextMenu")) {
			bool isAsset = m_Path.source == Core::FileSource::Asset;

			VXM_CORE_ASSERT(isAsset, "Should be in the Asset folder.");

			if(ImGui::MenuItem("New Folder")) {
				ClearFileNameBuffer();
				std::strcpy(m_FileNameBuffer.data(), "New Folder");
				popupToOpen = PopupToOpen::NewFolder;
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::MenuItem("New Source")) {
				ClearFileNameBuffer();
				std::strcpy(m_FileNameBuffer.data(), "New ShaderSource");
				popupToOpen = PopupToOpen::NewShaderSource;
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::MenuItem("New Shader")) {
				ClearFileNameBuffer();
				std::strcpy(m_FileNameBuffer.data(), "New Shader");
				popupToOpen = PopupToOpen::NewShader;
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::MenuItem("New Material")) {
				ClearFileNameBuffer();
				std::strcpy(m_FileNameBuffer.data(), "New Material");
				popupToOpen = PopupToOpen::NewMaterial;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		switch (popupToOpen) {
			case PopupToOpen::NewFolder: {
				ImGui::OpenPopup("ContentBrowserCreateFolder");
				break;
			}
			case PopupToOpen::NewShaderSource: {
				ImGui::OpenPopup("ContentBrowserCreateShaderSource");
				break;
			}
			case PopupToOpen::NewShader: {
				ImGui::OpenPopup("ContentBrowserCreateShader");
				break;
			}
			case PopupToOpen::NewMaterial: {
				ImGui::OpenPopup("ContentBrowserCreateMaterial");
				break;
			}
			default:
			{}
		}


		if (ImGui::BeginPopupModal("ContentBrowserCreateFolder")) {

			ImGui::InputText("New Folder Name", m_FileNameBuffer.data(), m_FileNameBuffer.size());

			if(ImGui::Button("Create Folder")) {
				auto newFolder = m_Path;
				newFolder.path /= m_FileNameBuffer.data();
				if (!FileSystem::Exist(newFolder)) {
					std::filesystem::create_directory(newFolder);
				}
				else {
					VXM_CORE_ERROR("A folder named 'New Folder' already exist here.");
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if(ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("ContentBrowserCreateMaterial")) {

			ImGui::InputText("New Material Name", m_FileNameBuffer.data(), m_FileNameBuffer.size()-1);

			if(ImGui::Button("Create Material")) {
				auto newMaterialPath = m_Path;
				newMaterialPath.path /= m_FileNameBuffer.data() + std::string(MaterialExtension);
				if (!FileSystem::Exist(newMaterialPath)) {
					Ref<Material> material = assetManager->CreateAsset<Material>(newMaterialPath, newMaterialPath.path.stem().string());
					if(material) {
						MaterialSerializer::ExportEditorMaterial(assetManager->GetMetadata(material->Handle), material);
					}
				}
				else {
					VXM_CORE_ERROR("A material named '{0}' already exist here.", m_FileNameBuffer.data());
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if(ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("ContentBrowserCreateShader")) {

			ImGui::InputText("New Shader Name", m_FileNameBuffer.data(), m_FileNameBuffer.size()-1);

			if(ImGui::Button("Create Graphic Shader")) {
				auto newShaderPath = m_Path;
				newShaderPath.path /= m_FileNameBuffer.data() + std::string(ShaderExtension);
				if (!FileSystem::Exist(newShaderPath)) {
					Ref<Shader> shader = Shader::Create(newShaderPath.path.stem().string(), std::unordered_map<ShaderType, ShaderSourceField>{});
					if(assetManager->AddAsset(shader, newShaderPath)) {
						ShaderSerializer::ExportEditorGraphicShader(assetManager->GetMetadata(shader->Handle), shader);
					}
				}
				else {
					VXM_CORE_ERROR("A graphic shader named '{0}' already exist here.", m_FileNameBuffer.data());
				}
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::Button("Create Compute Shader")) {
				auto newShaderPath = m_Path;
				newShaderPath.path /= m_FileNameBuffer.data() + std::string(ComputeShaderExtension);
				if (!FileSystem::Exist(newShaderPath)) {
					Ref<ComputeShader> shader = ComputeShader::Create(newShaderPath.path.stem().string(), {});
					if(assetManager->AddAsset(shader, newShaderPath)) {
						ShaderSerializer::ExportEditorComputeShader(assetManager->GetMetadata(shader->Handle), shader);
					}
				}
				else {
					VXM_CORE_ERROR("A compute shader named '{0}' already exist here.", m_FileNameBuffer.data());
				}
				ImGui::CloseCurrentPopup();
			}


			ImGui::SameLine();
			if(ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("ContentBrowserCreateShaderSource")) {
			static ShaderType s_Type = ShaderType::None;
			ImGui::InputText("New Shader Source Name", m_FileNameBuffer.data(), m_FileNameBuffer.size()-1);

			ShaderImGui::ShaderTypeCombo("Shader Type", &s_Type);
			//TODO: Shader Type

			if(ImGui::Button("Create ShaderSource")) {
				auto newShaderSourcePath = m_Path;
				newShaderSourcePath.path /= m_FileNameBuffer.data();
				switch (s_Type) {
					case ShaderType::None: {
						newShaderSourcePath.path += ".glsl";
						break;
					}
					case ShaderType::COMPUTE_SHADER: {
						newShaderSourcePath.path += ".compute";
						break;
					}
					case ShaderType::VERTEX_SHADER: {
						newShaderSourcePath.path += ".vert";
						break;
					}
					case ShaderType::TESS_CONTROL_SHADER: {
						newShaderSourcePath.path += ".tessco";
						break;
					}
					case ShaderType::TESS_EVALUATION_SHADER: {
						newShaderSourcePath.path += ".tessev";
						break;
					}
					case ShaderType::GEOMETRY_SHADER: {
						newShaderSourcePath.path += ".geom";
						break;
					}
					case ShaderType::FRAGMENT_SHADER: {
						newShaderSourcePath.path += ".frag";
						break;
					}
				}

				if (!FileSystem::Exist(newShaderSourcePath)) {
					Ref<ShaderSource> shaderSource = assetManager->CreateAsset<FileShaderSource>(newShaderSourcePath, s_Type, newShaderSourcePath, c_DefaultShader);
					if(shaderSource) {
						auto strRef = shaderSource->GetString();
						FileSystem::Write(newShaderSourcePath, strRef->c_str(), strRef->size());
						ShaderSerializer::ExportEditorShaderSource(assetManager->GetMetadata(shaderSource->Handle), shaderSource);
					}
				}
				else {
					VXM_CORE_ERROR("A shader source named '{0}' already exist here.", m_FileNameBuffer.data());
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if(ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
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
				std::string stem = path.stem().string();

				if (filename.ends_with(".meta")) {
					continue;
				}

				if(filename.starts_with("vxm_ignore")) {
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
				else if (state == STATE_HOVERED) {
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
						ImGui::OpenPopup(pathStr.c_str());
					}
					else if (AssetImporter::GetAssetType(assetPath) != AssetType::None) {
						Ref<EditorAssetManager> assetManager = Project::GetActive()->GetEditorAssetManager();
						AssetHandle handle = NullAssetHandle;
						if(assetManager->IsAssetImported(assetPath, &handle) && ImGui::BeginDragDropSource()) {
							AssetMetadata metadata = assetManager->GetMetadata(handle);
							std::string payloadID = AssetTypeToPayloadID(metadata.Type);
							VXM_CORE_ASSERT(payloadID.size() < 32, "The payloadID is not high.");
							ImGui::SetDragDropPayload(payloadID.c_str(), &metadata.Handle, sizeof(metadata.Handle), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}
				}

				bool shouldRename = false;

				if (ImGui::BeginPopup(pathStr.c_str())) {
					auto assetManager = Project::GetActive()->GetEditorAssetManager();
					auto metadata = assetManager->GetMetadata(assetPath);

					if (ImGui::MenuItem("Delete")) {
						if (isFile && metadata) {
							VXM_CORE_INFO("Delete Asset '{0}'", assetPath.string());
							assetManager->RemoveAsset(metadata.Handle);
						} else if (isDirectory){
							VXM_CORE_INFO("Delete Folder '{0}'", assetPath.string());
							for (const auto& recurse_entry : fs::recursive_directory_iterator(assetPath)) {
								if(recurse_entry.is_directory()) continue;

								auto childPath = recurse_entry.path();

								auto assetManager = Project::GetActive()->GetEditorAssetManager();
								auto childMetadata = assetManager->GetMetadata(Path::GetPath(childPath));
								if(childMetadata) {
									VXM_CORE_INFO("Delete Asset '{0}'", childPath.string());
									assetManager->RemoveAsset(childMetadata.Handle);
									std::filesystem::remove(childPath);
								}
							}
						} else {
							VXM_CORE_INFO("Delete '{0}'", assetPath.string());
						}

						uint64_t numberThingDeleted = std::filesystem::remove_all(assetPath);
						ImGui::CloseCurrentPopup();
					}
					if (isInData && ImGui::MenuItem("Rename")) {
						shouldRename = true;
						ImGui::CloseCurrentPopup();
					}
					if(isFile) {
						if (!metadata)// && AssetImporter::GetAssetType(assetPath) != AssetType::None && ImGui::MenuItem("Import"))
						{
							auto possibleTypes = AssetImporter::GetPossibleAssetTypes(assetPath);
							for (auto type : possibleTypes) {
								std::string name = "Import as " + AssetTypeToString(type);
								if (ImGui::MenuItem(name.c_str())) {
									auto asset = assetManager->ImportAsset(assetPath, type);
									if (asset) {
										VXM_CORE_INFO("Asset '{0}' was successfully imported.", filename);
									}
									else {
										VXM_CORE_ERROR("Asset '{0}' failed to be imported.", filename);
									}
									ImGui::CloseCurrentPopup();
								}
							}
						}
						else if (metadata)
						{
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

							if(s_MenuFunc.contains(metadata.Type)) {
								bool shouldClose = s_MenuFunc.at(metadata.Type)(metadata);
								if(shouldClose) {
									ImGui::CloseCurrentPopup();
								}
							}
						}
					}
					ImGui::EndPopup();
				}

				auto id = "Rename " + filename;
				if(shouldRename) {
					ClearFileNameBuffer();
					std::strcpy(m_FileNameBuffer.data(), isDirectory ? filename.c_str() : stem.c_str());
					ImGui::OpenPopup(id.c_str());
				}

				if(ImGui::BeginPopupModal(id.c_str())){

					if(ImGui::Button("Reset")) {
						ClearFileNameBuffer();
						std::strcpy(m_FileNameBuffer.data(), isDirectory ? filename.c_str() : stem.c_str());
					}
					ImGui::SameLine();
					ImGui::InputText("File Name", m_FileNameBuffer.data(), m_FileNameBuffer.size());

					if(ImGui::Button("Rename")) {
						if(isDirectory) {
							auto fullAssetPath = assetPath.GetFullPath();
							auto newPath = fullAssetPath.parent_path() / m_FileNameBuffer.data();
							VXM_CORE_INFO("Rename directory '{0}' in '{1}'", fullAssetPath.string(), newPath.string());
							for (const auto& recurse_entry : fs::recursive_directory_iterator(fullAssetPath)) {
								if(recurse_entry.is_directory()) continue;

								auto childPath = recurse_entry.path();
								auto relChildPath = fs::relative(childPath, fullAssetPath);
								auto newChildPath = newPath / relChildPath;

								auto assetManager = Project::GetActive()->GetEditorAssetManager();
								auto metadata = assetManager->GetMetadata(Path::GetPath(childPath));
								if(metadata) {
									VXM_CORE_INFO("Rename asset '{0}' in '{1}'", childPath.string(), newChildPath.string());
									assetManager->SetPath(metadata.Handle, Path::GetPath(newChildPath));
								}
							}
							fs::rename(fullAssetPath, newPath);
						} else {
							auto assetManager = Project::GetActive()->GetEditorAssetManager();
							auto metadata = assetManager->GetMetadata(assetPath);

							auto extension = assetPath.path.extension();
							auto newPath = assetPath;
							newPath.path = assetPath.path.parent_path() / m_FileNameBuffer.data();
							newPath.path += extension;
							fs::rename(assetPath, newPath);
							if(metadata) {
								VXM_CORE_INFO("Rename asset '{0}' in '{1}'", assetPath.string(), newPath.string());
								assetManager->SetPath(metadata.Handle, newPath);
							}
						}
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel")) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
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