//
// Created by ianpo on 29/10/2023.
//

#pragma once

#include "Voxymore/Project/Project.hpp"


namespace Voxymore::Core
{
	class ProjectSerializer
	{
	private:
		Ref<Project> m_Handle;
	public:
		ProjectSerializer(Ref<Project>);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	};

} // namespace Voxymore::Core

