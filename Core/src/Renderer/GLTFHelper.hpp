//
// Created by Sayama on 28/09/2023.
//

#pragma once

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Shader.hpp"

#include <tiny_gltf.h>

#include "Voxymore/Renderer/ShaderDataType.hpp"

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
		static const size_t PrimitiveAttributeCount = 7;

		enum AccessorType : int
		{
			SCALAR = TINYGLTF_TYPE_SCALAR,
			VEC2 = TINYGLTF_TYPE_VEC2,
			VEC3 = TINYGLTF_TYPE_VEC3,
			VEC4 = TINYGLTF_TYPE_VEC4,
			MAT2 = TINYGLTF_TYPE_MAT2,
			MAT3 = TINYGLTF_TYPE_MAT3,
			MAT4 = TINYGLTF_TYPE_MAT4,
		};

		// Values from gltf specification.
		enum BufferTarget
		{
			ARRAY_BUFFER = 34962,
			ELEMENT_ARRAY_BUFFER = 34963,
		};

		// Values from gltf specification.
		enum ComponentType
		{
			SignedByte = 5120,
			UnsignedByte = 5121,
			SignedShort = 5122,
			UnsignedShort = 5123,
			// SignedInt = 5124, // Deprecated ???
			UnsignedInt = 5125,
			Float = 5126,
		};

		// Values from gltf specification.
		enum MeshRenderMode
		{
			POINTS = 0,
			LINES = 1,
			LINE_LOOP = 2,
			LINE_STRIP = 3,
			TRIANGLES = 4,
			TRIANGLE_STRIP = 5,
			TRIANGLE_FAN = 6,
		};

		class Helper
		{
		private:
		public:
			static std::string GetPrimitiveAttributeString(GLTF::PrimitiveAttribute attribute, int index = 0);
			static GLTF::AccessorType GetAssociatedAccessor(GLTF::PrimitiveAttribute attribute);
			static GLTF::AccessorType GetAccessorType(const std::string& accessorType);
			static int GetComponentCount(GLTF::AccessorType accessorType);
			static int GetComponentByteCount(GLTF::ComponentType componentType);

			static std::string ComponentTypeToString(ComponentType componentType);
			static ShaderDataType GetShaderDataType(GLTF::PrimitiveAttribute accessor, int colorCount = 4);
			static glm::mat4 GetMatrix(const tinygltf::Node& node);

			static std::string AccessorTypeToString(AccessorType accessorType);

			static bool NodeHasMesh(const tinygltf::Node& node);
			static tinygltf::Mesh &GetMesh(tinygltf::Model &model, const tinygltf::Node &node);
			static bool PrimitiveHasAttribute(const tinygltf::Primitive& primitive, GLTF::PrimitiveAttribute attribute, int index = 0);
			static GLTF::PrimitiveAttribute GetPrimitiveAttribute(const std::string &attributeStr);
		};
	}

    void FlipStbi();
	void UnflipStbi();
} // namespace Voxymore::Core

