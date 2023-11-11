//
// Created by ianpo on 11/11/2023.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include <unordered_map>

namespace Voxymore::Core
{

	class Assets
	{
	private:
		enum class FileType : int
		{
			None = 0,
			Texture,
			Model,
			Scene,
		};

		static std::unordered_map<FileType, std::vector<std::string>> s_FileTypeExtensions;
		static std::unordered_map<Path, Ref<Scene>> s_Scenes;
		static std::unordered_map<Path, Ref<Model>> s_Models;
		static std::unordered_map<Path, Ref<Texture2D>> s_Textures;
	public:
		static void ReloadAll();
		static void ReloadAssets();
		static void ReloadEditorAssets();

		static Ref<Scene> GetScene(const Path& path);
		static Ref<Model> GetModel(const Path& path);
		static Ref<Texture2D> GetTexture(const Path& path);

		static bool HasScene(const Path& path);
		static bool HasModel(const Path& path);
		static bool HasTexture(const Path& path);

		static bool LoadTexture(const Path& filePath);
		static bool LoadModel(const Path& filePath);
		static bool LoadScene(const Path& filePath);
	private:
		static void ReloadFolder(const std::filesystem::path& path);
	private:
		static FileType GetFileType(const std::filesystem::path& filePath);
		static FileType GetFileType(const Path& filePath);
		static void CleanLoadedAssets();
	};

} // namespace Voxymore::Core

