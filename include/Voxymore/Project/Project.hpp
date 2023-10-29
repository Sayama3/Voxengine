//
// Created by Sayama on 26/10/2023.
//

#pragma once

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include <string>
#include <filesystem>

namespace Voxymore::Core
{
	struct ProjectParameters
	{
		std::string name = "Untitled";

		std::filesystem::path projectDirectory;

		Path startScene;

		//TODO: Add script path once i've got scripting (i.e. C#/Lua/...).
	};

	class Project
	{
	public:
		Project();
		Project(ProjectParameters parameters);

		~Project();

		static void LoadProject();


	private:
		ProjectParameters m_Parameters;

		static Ref<Project> s_ActiveProject;
	};

} // namespace Voxymore::Core

