//
// Created by Sayama on 27/09/2023.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"

namespace Voxymore::Core
{

	class Model
	{
	public:
		static Ref<Model> CreateModel(const Path& path);
	};

} // namespace Voxymore::Core

