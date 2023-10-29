//
// Created by Sayama on 26/10/2023.
//

#include "Voxymore/Project/Project.hpp"


namespace Voxymore::Core
{
	Ref<Project> Project::s_ActiveProject;
	Project::Project()
	{

	}

	Project::Project(ProjectParameters parameters) : m_Parameters(std::move(parameters))
	{

	}

	Project::~Project()
	{

	}
} // namespace Voxymore::Core
