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
	namespace GLTF
	{
		enum PrimitiveAttribute
		{
			POSITION,
			NORMAL,
			TANGENT,
			TEXCOORD,
			COLOR,
			JOINTS,
			WEIGHT,
		};

		enum AccessorType : int
		{
			SCALAR = 1 << 0,
			VEC2 = 1 << 1,
			VEC3 = 1 << 2,
			VEC4 = 1 << 3,
			MAT2 = 1 << 4,
			MAT3 = 1 << 5,
			MAT4 = 1 << 6,
		};

	}


	class GLTFHelper
	{
	private:
	public:
		static std::string GetPrimitiveAttributeString(GLTFPrimitiveAttribute attribute, int index = 0);
		static GLTFAccessorType GetAssociatedAccessor(GLTFPrimitiveAttribute attribute);
		static GLTFAccessorType GetAccessorType(const std::string& accessorType);
		static int GetComponentCount(GLTFAccessorType accessorType);
		static glm::mat4 GetMatrix(const tinygltf::Node& node);
		static bool NodeHasMesh(const tinygltf::Node& node);
		static tinygltf::Mesh &GetMesh(tinygltf::Model &model, const tinygltf::Node &node);
		static bool PrimitiveHasAttribute(const tinygltf::Primitive& primitive, GLTFPrimitiveAttribute attribute, int index = 0);
	};

} // namespace Voxymore::Core

