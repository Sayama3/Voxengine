//
// Created by ianpo on 11/11/2023.
//

#include "Voxymore/Assets/Assets.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Scene/SceneManager.hpp"
#include "Voxymore/Scene/SceneSerializer.hpp"
#include <cctype>    // std::tolower
#include <string_view> // std::string_view

#define VXM_LOAD_ASSET_IF_FOUND(Maps, LoadIfExist, fileType, LoadFunc) if(LoadIfExist && FileSystem::Exist(path) && GetFileType(path) == fileType)\
		{\
			if(!LoadFunc(path)) return nullptr;\
			else {\
				UUID fileId = GetFileID(path);\
				auto it = Maps.find(fileId);\
				VXM_CORE_ASSERT(it != Maps.end(), "The scene '{0}' should have been loaded but is not.")\
				return it->second;\
			}\
		}\
		else\
		{\
			return nullptr;\
		}

namespace fs = std::filesystem;

namespace Voxymore::Core
{
	namespace Helper
	{

		bool ichar_equals(char a, char b)
		{
			VXM_PROFILE_FUNCTION();
			return std::tolower(static_cast<unsigned char>(a)) ==
				   std::tolower(static_cast<unsigned char>(b));
		}

		bool iequals(std::string_view lhs, std::string_view rhs)
		{
			VXM_PROFILE_FUNCTION();
			if(lhs.size() != rhs.size()) return false;
			for (int i = 0; i < lhs.size(); ++i) {
				if(!ichar_equals(lhs[i], rhs[i])) return false;
			}
			return true;
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
		VXM_PROFILE_FUNCTION();
		CleanLoadedAssets();

		ReloadAssets();
		ReloadEditorAssets();
	}

	void Assets::ReloadAssets()
	{
		VXM_PROFILE_FUNCTION();
		Path p = {FileSource::Asset, "./"};
		ReloadFolder(p.GetFullPath());
	}

	void Assets::ReloadEditorAssets()
	{
		VXM_PROFILE_FUNCTION();
		Path p = {FileSource::EditorAsset, "./"};
		ReloadFolder(p.GetFullPath());
	}

	void Assets::ReloadFolder(const fs::path &path)
	{
		VXM_PROFILE_FUNCTION();
		if(!fs::exists(path))
		{
			VXM_CORE_WARNING("The folder '{0}' doesn't exist. Exiting early.", path.string());
			return;
		}

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
		VXM_PROFILE_FUNCTION();
		return s_Scenes.contains(id) || SceneManager::HasScene(id);
	}

	Ref<Scene> Assets::GetScene(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		auto sceneIt = s_Scenes.find(id);
		if(sceneIt == s_Scenes.end()) {
			if(SceneManager::HasScene(id))
			{
				s_Scenes[id] = SceneManager::GetScene(id);
				return s_Scenes[id];
			}
			return nullptr;
		}
		return sceneIt->second;
	}

	bool Assets::HasModel(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		return s_Models.contains(id);
	}

	Ref<Model> Assets::GetModel(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		auto modelIt = s_Models.find(id);
		if(modelIt == s_Models.end()) return nullptr;
		return modelIt->second;
	}

	bool Assets::HasTexture(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		return s_Textures.contains(id);
	}

	Ref<Texture2D> Assets::GetTexture(UUID id)
	{
		VXM_PROFILE_FUNCTION();
		auto textureIt = s_Textures.find(id);
		if(textureIt == s_Textures.end()) return nullptr;
		return textureIt->second;
	}

	Assets::FileType Assets::GetFileType(const std::filesystem::path& filePath)
	{
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
		return GetFileType(filePath.GetFullPath());
	}

	bool Assets::LoadScene(const Path &filePath)
	{
		VXM_PROFILE_FUNCTION();
		UUID fileId;
		if(HasFileID(filePath)) {
			fileId = GetFileID(filePath);
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
		SetFileID(filePath, fileId);
		s_Scenes[fileId] = scene;
		return scene != nullptr;
	}

	bool Assets::LoadModel(const Path &filePath)
	{
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
		SceneManager::Clear();
		s_Scenes.clear();
		s_Models.clear();
		s_Textures.clear();
	}

	Ref<Scene> Assets::GetScene(const Path& path, bool loadIfExist)
	{
		VXM_PROFILE_FUNCTION();
		if(!HasFileID(path))
		{
			VXM_LOAD_ASSET_IF_FOUND(s_Scenes, loadIfExist, FileType::Scene, LoadScene)
		}
		UUID id = GetOrCreateFileID(path);
		auto it = s_Scenes.find(id);
		if(it == s_Scenes.end())
		{
			if(SceneManager::HasScene(id))
			{
				s_Scenes[id] = SceneManager::GetScene(id);
				return s_Scenes[id];
			}
			else
			{
				VXM_LOAD_ASSET_IF_FOUND(s_Scenes, loadIfExist, FileType::Scene, LoadScene)
			}
		}
		return it->second;
	}

	Ref<Model> Assets::GetModel(const Path& path, bool loadIfExist)
	{
		VXM_PROFILE_FUNCTION();
		if(!HasFileID(path))
		{
			VXM_LOAD_ASSET_IF_FOUND(s_Models, loadIfExist, FileType::Model, LoadModel)
		}
		UUID id = GetOrCreateFileID(path);
		auto it = s_Models.find(id);
		if(it == s_Models.end())
		{
			VXM_LOAD_ASSET_IF_FOUND(s_Models, loadIfExist, FileType::Model, LoadModel)
		}
		return it->second;
	}

	Ref<Texture2D> Assets::GetTexture(const Path& path, bool loadIfExist)
	{
		VXM_PROFILE_FUNCTION();
		if(!HasFileID(path))
		{
			VXM_LOAD_ASSET_IF_FOUND(s_Textures, loadIfExist, FileType::Texture, LoadTexture)
		}
		UUID id = GetOrCreateFileID(path);
		auto it = s_Textures.find(id);
		if(it == s_Textures.end())
		{
			VXM_LOAD_ASSET_IF_FOUND(s_Textures, loadIfExist, FileType::Texture, LoadTexture)
		}
		return it->second;
	}

	bool Assets::HasScene(const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		if(!HasFileID(path)) return false;
		UUID id = GetFileID(path);
		return HasScene(id);
	}
	bool Assets::HasModel(const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		if(!HasFileID(path)) return false;
		UUID id = GetFileID(path);
		return HasModel(id);
	}
	bool Assets::HasTexture(const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		if(!HasFileID(path)) return false;
		UUID id = GetFileID(path);
		return HasTexture(id);
	}

	UUID Assets::GetOrCreateFileID(Path filePath)
	{
		VXM_PROFILE_FUNCTION();
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

	UUID Assets::GetFileID(Path filePath)
	{
		VXM_PROFILE_FUNCTION();
		UUID id;
		filePath.path += ".meta";
		VXM_CORE_ASSERT(FileSystem::Exist(filePath), "The fileID '{0}' doesn't exist.", filePath.GetFullPath().string());
		YAML::Node node = FileSystem::ReadFileAsYAML(filePath);
		VXM_CORE_ASSERT(node["UUID"], "The fileID '{0}' doesn't have a 'UUID' value.", filePath.GetFullPath().string());
		return node["UUID"].as<uint64_t>();
	}

	void Assets::SetFileID(Path filePath, UUID id)
	{
		VXM_PROFILE_FUNCTION();
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
		VXM_PROFILE_FUNCTION();
		filePath.path += ".meta";
		if(!FileSystem::Exist(filePath)) return false;
		YAML::Node node = FileSystem::ReadFileAsYAML(filePath);
		auto idNode = node["UUID"];
		return idNode && idNode.IsScalar();
	}
}// namespace Voxymore::Core