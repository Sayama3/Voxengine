//
// Created by Sayama on 26/10/2023.
//

#pragma once

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Assets/AssetManagerBase.hpp"
#include "Voxymore/Assets/EditorAssetManager.hpp"
#include "Voxymore/Assets/RuntimeAssetManager.hpp"


namespace Voxymore::Core
{
	using void_func_ptr = std::function<void()>;

	struct ProjectConfig {
		std::string name = "Untitled";

		std::filesystem::path assetDirectory = "Assets";
		std::filesystem::path cacheDirectory = "Cache";
		std::filesystem::path systemDirectory = "Systems";

		std::optional<UUID> startSceneId;

		//TODO: Add script path once i've got scripting (i.e. C#/Lua/...).
	};


	class Project
	{
	public: // All the static methods
		inline static Ref<Project> GetActive() { return s_ActiveProject; }
		[[deprecated("use 'GetActive' instead.")]]
		inline static Ref<Project> Get() { return s_ActiveProject; }

		static std::filesystem::path GetAssetDirectory();
		static std::filesystem::path GetCacheDirectory();
		static std::filesystem::path GetSystemsDirectory();
		static const std::filesystem::path& GetProjectFilePath();

		static void ResetMainScene();
		static void SetMainScene(UUID id);
		static void SetMainScene(const Scene& scene);
		static void SetMainScene(const Ref<Scene>& scene);
		static UUID GetMainScene();

		static const ProjectConfig& GetConfig();
		inline static bool ProjectIsLoaded() { return s_ActiveProject != nullptr; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path&);
		static bool SaveActive(const std::filesystem::path& path);
		static bool SaveActive();

		static UUID AddOnLoad(const void_func_ptr&);
		static void RemoveOnLoad(UUID id);
	public: // all the public members
		Project();
		Project(ProjectConfig parameters);
		~Project();

		[[nodiscard]] inline Ref<AssetManagerBase> GetAssetManager() { return m_AssetManager; }

		[[nodiscard]] inline Ref<EditorAssetManager> GetEditorAssetManager() {
			VXM_PROFILE_FUNCTION();
			return CastPtr<EditorAssetManager>(m_AssetManager);
		}

		[[nodiscard]] inline Ref<RuntimeAssetManager> GetRuntimeAssetManager() {
			VXM_PROFILE_FUNCTION();
			return CastPtr<RuntimeAssetManager>(m_AssetManager);
		}
	private: // the rest of the private things
		void CallOnLoad();
		static std::unordered_map<UUID, void_func_ptr>& GetOnLoad();
	private:
		[[nodiscard]]std::filesystem::path GetAsset() const;
		[[nodiscard]]std::filesystem::path GetCache() const;
		[[nodiscard]]std::filesystem::path GetSystems() const;
		[[nodiscard]]const std::filesystem::path& GetFilePath() const;
	private:
		std::filesystem::path m_ProjectPath = "./Project.vxm";
		ProjectConfig m_Config;
		Ref<AssetManagerBase> m_AssetManager;
	private:
		static std::unordered_map<UUID, void_func_ptr>* s_OnLoad;
		static Ref<Project> s_ActiveProject;
		friend class ProjectSerializer;
	};

} // namespace Voxymore::Core

