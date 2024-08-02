//
// Created by Sayama on 26/10/2023.
//

#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Project/ProjectSerializer.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Physics/PhysicsDebugRenderer.hpp"
#include <algorithm>


namespace Voxymore::Core
{
	std::unordered_map<AssetHandle, void_func_ptr>* Project::s_OnLoad = nullptr;
	Ref<Project> Project::s_ActiveProject = nullptr;

	void TryLoadGizmoMaterial()
	{
		if(!Project::ProjectIsLoaded()) return;
		const ProjectConfig& config = Project::GetConfig();
		if(!config.gizmoMaterialId.has_value()) return;
		MaterialField gizmoMat {config.gizmoMaterialId.value()};
		if(!gizmoMat.IsValid()) return;
		VXM_CORE_INFO("Assigning a new Gizmo Material to the PhysicsDebugRenderer.");
		PhysicsDebugRenderer::s_Material = gizmoMat.GetAsset();
	}

	std::unordered_map<AssetHandle, void_func_ptr>& Project::GetOnLoad()
	{
		VXM_PROFILE_FUNCTION();
		if(Project::s_OnLoad == nullptr) {
			s_OnLoad = new std::unordered_map<AssetHandle, void_func_ptr>();
			s_OnLoad->insert({AssetHandle{}, &TryLoadGizmoMaterial});
		}
		return *Project::s_OnLoad;
	}

	AssetHandle Project::AddOnLoad(const void_func_ptr& func)
	{
		VXM_PROFILE_FUNCTION();
		AssetHandle id;
		GetOnLoad()[id] = func;
		return id;
	}

	void Project::RemoveOnLoad(AssetHandle id)
	{
		VXM_PROFILE_FUNCTION();
		const auto it = GetOnLoad().find(id);
		if (it != GetOnLoad().end())
		{
			GetOnLoad().erase(it);
		}
	}

	Project::Project()
	{
		if(m_ProjectPath.is_relative()) {
			m_ProjectPath = std::filesystem::current_path() / m_ProjectPath;
		}
	}

	Project::Project(ProjectConfig parameters) : m_Config(std::move(parameters))
	{
		if(m_ProjectPath.is_relative()) {
			m_ProjectPath = std::filesystem::current_path() / m_ProjectPath;
		}
	}

	Project::~Project()
	{

	}

	Ref<Project> Project::New()
	{
		VXM_PROFILE_FUNCTION();
		s_ActiveProject = CreateRef<Project>();
		s_ActiveProject->CallOnLoad();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path &path)
	{
		VXM_PROFILE_FUNCTION();
		Ref<Project> project = CreateRef<Project>();
		auto old = s_ActiveProject;
		s_ActiveProject = project;

		ProjectSerializer ps(project);
		if(ps.Deserialize(path))
		{
			s_ActiveProject->CallOnLoad();
			return s_ActiveProject;
		} else {
			s_ActiveProject = old;
		}
		return nullptr;
	}

	bool Project::SaveActive()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project was not loaded.");
		ProjectSerializer ps(s_ActiveProject);
		return ps.Serialize(s_ActiveProject->m_ProjectPath);
	}

	bool Project::SaveActive(const std::filesystem::path &path)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project was not loaded.");
		VXM_CORE_ASSERT(!path.empty(), "When given, the path should not be empty.");
		ProjectSerializer ps(s_ActiveProject);
		return ps.Serialize(path);
	}

	void Project::CallOnLoad()
	{
		VXM_PROFILE_FUNCTION();
		for (auto&&[id, func] : GetOnLoad())
		{
			func();
		}
	}

	std::filesystem::path Project::GetAsset() const
	{
		VXM_PROFILE_FUNCTION();
		if(m_Config.assetDirectory.is_relative())
			return m_ProjectPath.parent_path() / m_Config.assetDirectory;
		else
			return m_Config.assetDirectory;
	}

	std::filesystem::path Project::GetCache() const
	{
		VXM_PROFILE_FUNCTION();
		if(m_Config.cacheDirectory.is_relative())
			return m_ProjectPath.parent_path() / m_Config.cacheDirectory;
		else
			return m_Config.cacheDirectory;
	}

	std::filesystem::path Project::GetSystems() const
	{
		VXM_PROFILE_FUNCTION();
		if(m_Config.systemDirectory.is_relative())
			return m_ProjectPath.parent_path() / m_Config.systemDirectory;
		else
			return m_Config.systemDirectory;
	}

	std::filesystem::path Project::GetAssetRegistry() const
	{
		VXM_PROFILE_FUNCTION();
		if(m_Config.assetRegistryPath.is_relative())
			return m_ProjectPath.parent_path() / m_Config.assetRegistryPath;
		else
			return m_Config.assetRegistryPath;
	}

	const std::filesystem::path& Project::GetFilePath() const
	{
		VXM_PROFILE_FUNCTION();
		return m_ProjectPath;
	}

	std::filesystem::path Project::GetAssetDirectory()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		return s_ActiveProject->GetAsset();
	}
	std::filesystem::path Project::GetCacheDirectory()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		return s_ActiveProject->GetCache();
	}
	std::filesystem::path Project::GetSystemsDirectory()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		return s_ActiveProject->GetSystems();
	}

	std::filesystem::path Project::GetAssetRegistryPath()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		return s_ActiveProject->GetAssetRegistry();
	}

	const std::filesystem::path& Project::GetProjectFilePath()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		return s_ActiveProject->GetFilePath();
	}

	const ProjectConfig& Project::GetConfig()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		return s_ActiveProject->m_Config;
	}

	void Project::ResetMainScene()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		s_ActiveProject->m_Config.startSceneId = {};
	}

	void Project::SetMainScene(AssetHandle id)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		s_ActiveProject->m_Config.startSceneId = id;
	}

	void Project::SetMainScene(const Scene& scene)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		s_ActiveProject->m_Config.startSceneId = scene.id();
	}

	void Project::SetMainScene(const Ref<Scene>& scene)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		s_ActiveProject->m_Config.startSceneId = scene->id();
	}

	AssetHandle Project::GetMainScene()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		VXM_CORE_ASSERT(s_ActiveProject->m_Config.startSceneId.has_value(), "Not Start Scene associated with the project.");
		return s_ActiveProject->m_Config.startSceneId.value();
	}


	void Project::SetDefaultMaterial(AssetHandle id)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		s_ActiveProject->m_Config.defaultMaterialId = id;
	}
	AssetHandle Project::GetDefaultMaterial()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		VXM_CORE_ASSERT(s_ActiveProject->m_Config.defaultMaterialId.has_value(), "Not Default Material associated with the project.");
		return s_ActiveProject->m_Config.defaultMaterialId.value();
	}
	void Project::SetDefaultShader(AssetHandle id)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		s_ActiveProject->m_Config.defaultShaderId = id;
	}
	AssetHandle Project::GetDefaultShader()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		VXM_CORE_ASSERT(s_ActiveProject->m_Config.defaultShaderId.has_value(), "Not Default Shader associated with the project.");
		return s_ActiveProject->m_Config.defaultShaderId.value();
	}

	void Project::SetGizmoMaterial(AssetHandle id)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		s_ActiveProject->m_Config.gizmoMaterialId = id;
		//TODO: #ifdef with a 'VXM_DRAW_GIZMO' to know if we compile this thing.
		MaterialField gizmoMat{id};
		if(gizmoMat) {
			VXM_CORE_INFO("Assigning a new Gizmo Material to the PhysicsDebugRenderer.");
			PhysicsDebugRenderer::s_Material = gizmoMat.GetAsset();
		}
	}

	AssetHandle Project::GetGizmoMaterial()
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project is not loaded yet.");
		VXM_CORE_ASSERT(s_ActiveProject->m_Config.gizmoMaterialId.has_value(), "Not Gizmo Material associated with the project.");
		return s_ActiveProject->m_Config.gizmoMaterialId.value();
	}

} // namespace Voxymore::Core
