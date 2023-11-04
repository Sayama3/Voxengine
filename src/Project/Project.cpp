//
// Created by Sayama on 26/10/2023.
//

#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Project/ProjectSerializer.hpp"
#include <algorithm>


namespace Voxymore::Core
{
	std::vector<VOID_FUNC_PTR>* Project::s_OnLoad = nullptr;
	Ref<Project> Project::s_ActiveProject;

	std::vector<VOID_FUNC_PTR>& Project::GetOnLoad()
	{
		if(Project::s_OnLoad == nullptr) {
			s_OnLoad = new std::vector<VOID_FUNC_PTR>();
		}
		return *Project::s_OnLoad;
	}

	void Project::AddOnLoad(NAMED_VOID_FUNC_PTR(func))
	{
		GetOnLoad().push_back(func);
	}

	void Project::RemoveOnLoad(NAMED_VOID_FUNC_PTR(func))
	{
		const auto it = std::find(GetOnLoad().cbegin(), GetOnLoad().cend(), func);
		if (it != GetOnLoad().end())
		{
			GetOnLoad().erase(it);
		}
	}

	Project::Project()
	{

	}

	Project::Project(ProjectConfig parameters) : m_Config(std::move(parameters))
	{

	}

	Project::~Project()
	{

	}

	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();
		s_ActiveProject->CallOnLoad();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path &path)
	{
		Ref<Project> project = CreateRef<Project>();
		ProjectSerializer ps(project);
		if(ps.Deserialize(path))
		{
			s_ActiveProject = project;
			s_ActiveProject->CallOnLoad();
			return s_ActiveProject;
		}
		return nullptr;
	}

	bool Project::SaveActive()
	{
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project was not loaded.");
		ProjectSerializer ps(s_ActiveProject);
		return ps.Serialize(s_ActiveProject->m_ProjectPath);
	}

	bool Project::SaveActive(const std::filesystem::path &path)
	{
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project was not loaded.");
		VXM_CORE_ASSERT(!path.empty(), "When given, the path should not be empty.");
		ProjectSerializer ps(s_ActiveProject);
		return ps.Serialize(path);
	}

	void Project::CallOnLoad()
	{
		for (NAMED_VOID_FUNC_PTR(func) : GetOnLoad())
		{
			func();
		}
	}

	std::filesystem::path Project::GetAsset() const
	{
		if(m_Config.assetDirectory.is_relative())
			return m_ProjectPath.parent_path() / m_Config.assetDirectory;
		else
			return m_Config.assetDirectory;
	}

	std::filesystem::path Project::GetCache() const
	{
		if(m_Config.cacheDirectory.is_relative())
			return m_ProjectPath.parent_path() / m_Config.cacheDirectory;
		else
			return m_Config.cacheDirectory;
	}

	std::filesystem::path Project::GetSystems() const
	{
		if(m_Config.systemDirectory.is_relative())
			return m_ProjectPath.parent_path() / m_Config.systemDirectory;
		else
			return m_Config.systemDirectory;
	}

	const std::filesystem::path& Project::GetFilePath() const
	{
		return m_ProjectPath;
	}

} // namespace Voxymore::Core
