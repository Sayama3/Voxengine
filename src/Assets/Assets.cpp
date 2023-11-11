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

	std::unordered_map<Path, Ref<Scene>> Assets::s_Scenes;
	std::unordered_map<Path, Ref<Model>> Assets::s_Models;
	std::unordered_map<Path, Ref<Texture2D>> Assets::s_Textures;

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

	bool Assets::HasScene(const Path& path)
	{
		return s_Scenes.contains(path);
	}

	Ref<Scene> Assets::GetScene(const Path& path)
	{
		auto sceneIt = s_Scenes.find(path);
		if(sceneIt == s_Scenes.end()) return nullptr;
		return sceneIt->second;
	}

	bool Assets::HasModel(const Path& path)
	{
		return s_Models.contains(path);
	}

	Ref<Model> Assets::GetModel(const Path& path)
	{
		auto modelIt = s_Models.find(path);
		if(modelIt == s_Models.end()) return nullptr;
		return modelIt->second;
	}

	bool Assets::HasTexture(const Path& path)
	{
		return s_Textures.contains(path);
	}

	Ref<Texture2D> Assets::GetTexture(const Path& path)
	{
		auto textureIt = s_Textures.find(path);
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
		if(s_Scenes.contains(filePath)) return true;
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
		s_Scenes[filePath] = scene;
		return scene != nullptr;
	}

	bool Assets::LoadModel(const Path &filePath)
	{
		if(s_Models.contains(filePath)) return true;

		VXM_CORE_ASSERT(ShaderLibrary::GetInstance().Exists("Default"), "No 'Default' shader in the ShaderLibrary.");

		Ref<Model> model = Model::CreateModel(filePath, ShaderLibrary::GetInstance().Get("Default"));
		if(model != nullptr)
		{
			s_Models[filePath] = model;
		}
		return model != nullptr;
	}

	bool Assets::LoadTexture(const Path& filePath)
	{
		if(s_Textures.contains(filePath)) return true;

		Ref<Texture2D> texture = Texture2D::Create(filePath);
		if(texture != nullptr)
		{
			s_Textures[filePath] = texture;
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

}// namespace Voxymore::Core