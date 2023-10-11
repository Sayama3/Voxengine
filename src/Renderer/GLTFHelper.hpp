//
// Created by Sayama on 28/09/2023.
//

#pragma once

#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Renderer/Shader.hpp"

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
		static const size_t PrimitiveAttributeCount = 7;

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

			static bool NodeHasMesh(const tinygltf::Node& node);
			static tinygltf::Mesh &GetMesh(tinygltf::Model &model, const tinygltf::Node &node);
			static bool PrimitiveHasAttribute(const tinygltf::Primitive& primitive, GLTF::PrimitiveAttribute attribute, int index = 0);
			static GLTF::PrimitiveAttribute GetPrimitiveAttribute(const std::string &attributeStr);
		};
	}




} // namespace Voxymore::Core

