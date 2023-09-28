//
// Created by Sayama on 28/09/2023.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Math.hpp"
#include <tiny_gltf.h>

namespace Voxymore::Core
{
	enum class GLTFAccessorType
	{
		POSITION,
		NORMAL,
		TANGENT,
		TEXCOORD,
		COLOR,
		JOINTS,
		WEIGHT,
	};

	class GLTFHelper
	{
	public:
		GLTFAccessorType GetAccessor(const std::string& accessorType)
		{

		}
		glm::mat4 GetMatrix(const tinygltf::Node& node);
	};

} // namespace Voxymore::Core

