//
// Created by Sayama on 26/10/2023.
//

#pragma once

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Scene/Scene.hpp"

//#define VOID_FUNC_PTR void(*)()
//#define NAMED_VOID_FUNC_PTR(name) void(*name)()
//#define CONST_REF_NAMED_VOID_FUNC_PTR(name) void(*name)()
//#define CONST_REF_VOID_FUNC_PTR void(*)()
#define VOID_FUNC_PTR std::function<void()>
#define NAMED_VOID_FUNC_PTR(name) std::function<void()> name
#define CONST_REF_NAMED_VOID_FUNC_PTR(name) const std::function<void()>& name
#define CONST_REF_VOID_FUNC_PTR const std::function<void()>&

namespace Voxymore::Core
{
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
	private:
		static std::unordered_map<UUID, VOID_FUNC_PTR>* s_OnLoad;
		static Ref<Project> s_ActiveProject;
		friend class ProjectSerializer;
	public:
		Project();
		Project(ProjectConfig parameters);
		~Project();

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

		static UUID AddOnLoad(CONST_REF_VOID_FUNC_PTR);
		static void RemoveOnLoad(UUID id);

	private:
		void CallOnLoad();
		static std::unordered_map<UUID, VOID_FUNC_PTR>& GetOnLoad();
	private:
		std::filesystem::path GetAsset() const;
		std::filesystem::path GetCache() const;
		std::filesystem::path GetSystems() const;
		const std::filesystem::path& GetFilePath() const;

	private:
		std::filesystem::path m_ProjectPath = "./Project.vxm";
		ProjectConfig m_Config;
	};

} // namespace Voxymore::Core

