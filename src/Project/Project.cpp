//
// Created by Sayama on 26/10/2023.
//

#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Project/ProjectSerializer.hpp"


namespace Voxymore::Core
{
	Ref<Project> Project::s_ActiveProject;

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
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path &path)
	{
		Ref<Project> project = CreateRef<Project>();
		ProjectSerializer ps(s_ActiveProject);
		if(ps.Deserialize(path))
		{
			s_ActiveProject = project;
			return s_ActiveProject;
		}
		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path &path)
	{
		VXM_CORE_ASSERT(s_ActiveProject, "The Active Project was not loaded.");
		ProjectSerializer ps(s_ActiveProject);
		return ps.Serialize(path.empty() ? s_ActiveProject->m_ProjectPath : path);
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
