//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/Assets/Assets.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Scene/SceneManager.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"
#include <cctype>    // std::tolower
#include <algorithm> // std::ranges::equal or std::equal
#include <string_view> // std::string_view

namespace fs = std::filesystem;

namespace Voxymore::Core
{
	namespace Helper
	{

		bool ichar_equals(char a, char b)
		{
			return std::tolower(static_cast<unsigned char>(a)) ==
				   std::tolower(static_cast<unsigned char>(b));
		}

		bool iequals(std::string_view lhs, std::string_view rhs)
		{
			return std::ranges::equal(lhs, rhs, ichar_equals);
		}
	}
	std::unordered_map<Assets::FileType, std::vector<std::string>> Assets::s_FileTypeExtensions = {
			{Assets::FileType::Scene, {".vxm"} },
			{Assets::FileType::Model, {".gltf", ".glb"} },
			{Assets::FileType::Texture, {".png", ".jpg", "jpeg", ".bmp"} },
	};

	std::unordered_map<UUID, Ref<Scene>> Assets::s_Scenes;
	std::unordered_map<UUID, Ref<Model>> Assets::s_Models;
	std::unordered_map<UUID, Ref<Texture2D>> Assets::s_Textures;

	void Assets::ReloadAll()
	{
		CleanLoadedAssets();

		ReloadAssets();
		ReloadEditorAssets();
	}

	void Assets::ReloadAssets()
	{
		Path p = {FileSource::Asset, "./"};
		ReloadFolder(p.GetFullPath());
	}

	void Assets::ReloadEditorAssets()
	{
		Path p = {FileSource::EditorAsset, "./"};
		ReloadFolder(p.GetFullPath());
	}

	void Assets::ReloadFolder(const fs::path &path)
	{
		VXM_CORE_ASSERT(fs::is_directory(path), "The path '{0}' must be a folder.", path.string());
		for (const fs::directory_entry& dirEntry : fs::recursive_directory_iterator(path))
		{
			if(!dirEntry.is_regular_file()) continue;
			const fs::path & filePath = dirEntry.path();
			Path vxmFilePath = Path::GetPath(filePath);
			auto fileType = GetFileType(vxmFilePath);
			switch (fileType)
			{
				case FileType::None: continue;
				case FileType::Texture: {
					if(LoadTexture(vxmFilePath))
					{
						//TODO: Do something.
					}
					break;
				}
				case FileType::Model: {
					if(LoadModel(vxmFilePath))
					{
						//TODO: Do something.
					}
					break;
				}
				case FileType::Scene: {
					if(LoadScene(vxmFilePath))
					{
					}
					break;
				}
			}
		}
	}

	bool Assets::HasScene(UUID id)
	{
		return s_Scenes.contains(id);
	}

	Ref<Scene> Assets::GetScene(UUID id)
	{
		auto sceneIt = s_Scenes.find(id);
		if(sceneIt == s_Scenes.end()) return nullptr;
		return sceneIt->second;
	}

	bool Assets::HasModel(UUID id)
	{
		return s_Models.contains(id);
	}

	Ref<Model> Assets::GetModel(UUID id)
	{
		auto modelIt = s_Models.find(id);
		if(modelIt == s_Models.end()) return nullptr;
		return modelIt->second;
	}

	bool Assets::HasTexture(UUID id)
	{
		return s_Textures.contains(id);
	}

	Ref<Texture2D> Assets::GetTexture(UUID id)
	{
		auto textureIt = s_Textures.find(id);
		if(textureIt == s_Textures.end()) return nullptr;
		return textureIt->second;
	}

	Assets::FileType Assets::GetFileType(const std::filesystem::path& filePath)
	{

		if(!filePath.has_extension()) return Assets::FileType::None;
		std::string fileExtention = filePath.extension().string();
		for (auto&&[fileSource, extentions] : Assets::s_FileTypeExtensions)
		{
			for (auto& extention : extentions)
			{
				if(Helper::iequals(extention, fileExtention)) return fileSource;
			}
		}
		return Assets::FileType::None;
	}

	Assets::FileType Assets::GetFileType(const Path &filePath)
	{
		return GetFileType(filePath.GetFullPath());
	}

	bool Assets::LoadScene(const Path &filePath)
	{
		UUID fileId;
		if(HasFileID(filePath)) {
			fileId = GetOrCreateFileID(filePath);
			if (s_Scenes.contains(fileId)) return true;
		}

		Ref<Scene> scene = nullptr;
		std::optional<UUID> id = SceneSerializer::GetSceneID(filePath.GetFullPath().string());

		if(!id.has_value()) return false;

		if(SceneManager::HasScene(id.value()))
		{
			scene = SceneManager::GetScene(id.value());
		}
		else
		{
			scene = SceneManager::CreateScene(filePath, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		}

		fileId = scene->GetID();
		s_Scenes[fileId] = scene;
		return scene != nullptr;
	}

	bool Assets::LoadModel(const Path &filePath)
	{
		UUID fileId = GetOrCreateFileID(filePath);
		if(s_Models.contains(fileId)) return true;

		VXM_CORE_ASSERT(ShaderLibrary::GetInstance().Exists("Default"), "No 'Default' shader in the ShaderLibrary.");

		Ref<Model> model = Model::CreateModel(filePath, ShaderLibrary::GetInstance().Get("Default"));
		if(model != nullptr)
		{
			s_Models[fileId] = model;
		}
		return model != nullptr;
	}

	bool Assets::LoadTexture(const Path& filePath)
	{
		UUID fileId = GetOrCreateFileID(filePath);
		if(s_Textures.contains(fileId)) return true;

		Ref<Texture2D> texture = Texture2D::Create(filePath);
		if(texture != nullptr)
		{
			s_Textures[fileId] = texture;
		}
		return texture != nullptr;
	}

	void Assets::CleanLoadedAssets()
	{
		SceneManager::Clear();
		s_Scenes.clear();
		s_Models.clear();
		s_Textures.clear();
	}

	Ref<Scene> Assets::GetScene(const Path& path)
	{
		if(!HasFileID(path)) return nullptr;
		UUID id = GetOrCreateFileID(path);
		auto it = s_Scenes.find(id);
		if(it == s_Scenes.end()) return nullptr;
		return it->second;
	}
	Ref<Model> Assets::GetModel(const Path& path)
	{
		if(!HasFileID(path)) return nullptr;
		UUID id = GetOrCreateFileID(path);
		auto it = s_Models.find(id);
		if(it == s_Models.end()) return nullptr;
		return it->second;
	}
	Ref<Texture2D> Assets::GetTexture(const Path& path)
	{
		if(!HasFileID(path)) return nullptr;
		UUID id = GetOrCreateFileID(path);
		auto it = s_Textures.find(id);
		if(it == s_Textures.end()) return nullptr;
		return it->second;
	}

	bool Assets::HasScene(const Path& path)
	{
		if(!HasFileID(path)) return false;
		UUID id = GetOrCreateFileID(path);
		return s_Scenes.contains(id);
	}
	bool Assets::HasModel(const Path& path)
	{
		if(!HasFileID(path)) return false;
		UUID id = GetOrCreateFileID(path);
		return s_Models.contains(id);
	}
	bool Assets::HasTexture(const Path& path)
	{
		if(!HasFileID(path)) return false;
		UUID id = GetOrCreateFileID(path);
		return s_Textures.contains(id);
	}

	UUID Assets::GetOrCreateFileID(Path filePath)
	{
		UUID id;
		filePath.path += ".meta";
		if(FileSystem::Exist(filePath))
		{
			YAML::Node node = FileSystem::ReadFileAsYAML(filePath);
			if(node["UUID"])
			{
				id = node["UUID"].as<uint64_t >();
			}
			else
			{
				node["UUID"] = std::to_string(id);
				YAML::Emitter out;
				out << node;
				FileSystem::WriteYamlFile(filePath, out);
			}
		}
		else
		{
			YAML::Emitter out;
			out << YAML::BeginMap;
			{
				out << KEYVAL("UUID", id);
				out << YAML::EndMap;
			}
			FileSystem::WriteYamlFile(filePath, out);
		}
		return id;
	}

	void Assets::SetFileID(Path filePath, UUID id)
	{
		filePath.path += ".meta";
		YAML::Emitter out;
		if(FileSystem::Exist(filePath))
		{
			YAML::Node node = FileSystem::ReadFileAsYAML(filePath);
			node["UUID"] = std::to_string(id);
			out << node;
		}
		else
		{
			out << YAML::BeginMap;
			{
				out << KEYVAL("UUID", id);
				out << YAML::EndMap;
			}
		}
		FileSystem::WriteYamlFile(filePath, out);
	}

	bool Assets::HasFileID(Path filePath)
	{
		filePath.path += ".meta";
		if(!FileSystem::Exist(filePath)) return false;
		YAML::Node node = FileSystem::ReadFileAsYAML(filePath);
		auto idNode = node["UUID"];
		return idNode && idNode.IsScalar();
	}

}// namespace Voxymore::Core