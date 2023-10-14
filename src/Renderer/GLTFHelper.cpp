//
// Created by Sayama on 28/09/2023.
//

#include "GLTFHelper.hpp"
#include "Voxymore/Core/Logger.hpp"

namespace Voxymore::Core
{
	namespace GLTF
	{
		glm::mat4 Helper::GetMatrix(const tinygltf::Node& node)
		{
			glm::mat4 matrix(1.0f);
			if(!node.matrix.empty())
			{
				for (int x = 0; x < 4; ++x) {
					for (int y = 0; y < 4; ++y) {
						matrix[x][y] = node.matrix[x + (y*4)];
					}
				}
			}
			else if(node.translation.size() == 3 || node.rotation.size() == 4 || node.scale.size() == 3)
			{
				glm::vec3 pos;
				if(node.translation.size() == 3) pos = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
				glm::quat rot;
				if(node.rotation.size() == 4) rot = glm::quat(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
				glm::vec3 scale;
				if(node.scale.size() == 3) scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
				Math::TRS(matrix, pos, rot, scale);
			}
			return matrix;
		}
		int Helper::GetComponentCount(GLTF::AccessorType accessorType)
		{
			int count = -1;
			switch (accessorType)
			{
				case GLTF::AccessorType::SCALAR: {count = 1; break;}
				case GLTF::AccessorType::VEC2: {count = 2; break;}
				case GLTF::AccessorType::VEC3: {count = 3; break;}
				case GLTF::AccessorType::VEC4: {count = 4; break;}
				case GLTF::AccessorType::MAT2: {count = 2*2; break;}
				case GLTF::AccessorType::MAT3: {count = 3*3; break;}
				case GLTF::AccessorType::MAT4: {count = 4*4; break;}
				default: { VXM_CORE_ASSERT(false, "Accessor type '{0}' is unknown.", (int)accessorType); break; }
			}
			return count;
		}
		GLTF::AccessorType Helper::GetAccessorType(const std::string &accessorType)
		{
			GLTF::AccessorType accessor;
			if(accessorType == "SCALAR") accessor = GLTF::AccessorType::SCALAR;
			else if(accessorType == "VEC2") accessor = GLTF::AccessorType::VEC2;
			else if(accessorType == "VEC3") accessor = GLTF::AccessorType::VEC3;
			else if(accessorType == "VEC4") accessor = GLTF::AccessorType::VEC4;
			else if(accessorType == "MAT2") accessor = GLTF::AccessorType::MAT2;
			else if(accessorType == "MAT3") accessor = GLTF::AccessorType::MAT3;
			else if(accessorType == "MAT4") accessor = GLTF::AccessorType::MAT4;
			else { VXM_CORE_ASSERT(false, "Accessor type '{0}' is unknown.", accessorType); }
			return accessor;
		}


		ShaderDataType Helper::GetShaderDataType(GLTF::PrimitiveAttribute accessor, int colorCount)
		{
			switch (accessor)
			{
				case GLTF::POSITION: return ShaderDataType::Float3;
				case GLTF::NORMAL: return ShaderDataType::Float3;
				case GLTF::TANGENT: return ShaderDataType::Float4;
				case GLTF::TEXCOORD: return ShaderDataType::Float2;
				case GLTF::COLOR: return colorCount == 4 ? ShaderDataType::Float4 : ShaderDataType::Float3;
				case GLTF::JOINTS: return ShaderDataType::Float3;
				case GLTF::WEIGHT: return ShaderDataType::Float3;
			}

			return ShaderDataType::None;
		}

		bool Helper::NodeHasMesh(const tinygltf::Node &node)
		{
			return node.mesh > -1;
		}

		tinygltf::Mesh&Helper::GetMesh(tinygltf::Model &model, const tinygltf::Node &node)
		{
			VXM_CORE_ASSERT(NodeHasMesh(node), "The node {0} must have a mesh.", node.name);
			return model.meshes[node.mesh];
		}
		bool Helper::PrimitiveHasAttribute(const tinygltf::Primitive &primitive, GLTF::PrimitiveAttribute attribute, int index)
		{
			std::string name = GetPrimitiveAttributeString(attribute, index);
			return primitive.attributes.contains(name);
		}
		std::string Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute attribute, int index)
		{
			std::string name;
			switch (attribute)
			{
				case GLTF::PrimitiveAttribute::POSITION: name = "POSITION"; break;
				case GLTF::PrimitiveAttribute::NORMAL: name = "NORMAL"; break;
				case GLTF::PrimitiveAttribute::TANGENT: name = "TANGENT"; break;
				case GLTF::PrimitiveAttribute::TEXCOORD: name = "TEXCOORD_" + std::to_string(index); break;
				case GLTF::PrimitiveAttribute::COLOR: name = "COLOR_" + std::to_string(index); break;
				case GLTF::PrimitiveAttribute::JOINTS: name = "JOINTS_" + std::to_string(index); break;
				case GLTF::PrimitiveAttribute::WEIGHT: name = "WEIGHT_" + std::to_string(index); break;
			}
			return name;
		}

		std::string Helper::ComponentTypeToString(ComponentType componentType)
		{
			switch (componentType)
			{
				case ComponentType::SignedByte: return "SignedByte";
				case ComponentType::UnsignedByte: return "UnsignedByte";
				case ComponentType::SignedShort: return "SignedShort";
				case ComponentType::UnsignedShort: return "UnsignedShort";
					//                case ComponentType::SignedInt: return "SignedInt";
				case ComponentType::UnsignedInt: return "UnsignedInt";
				case ComponentType::Float: return "Float";
			}
			return "Unknown";
		}

		GLTF::AccessorType Helper::GetAssociatedAccessor(GLTF::PrimitiveAttribute attribute)
		{
			switch (attribute)
			{
				case GLTF::PrimitiveAttribute::POSITION: return GLTF::AccessorType::VEC3;
				case GLTF::PrimitiveAttribute::NORMAL: return GLTF::AccessorType::VEC3;
				case GLTF::PrimitiveAttribute::TANGENT: return GLTF::AccessorType::VEC4;
				case GLTF::PrimitiveAttribute::TEXCOORD: return GLTF::AccessorType::VEC2;
				case GLTF::PrimitiveAttribute::COLOR: return static_cast<GLTF::AccessorType>(GLTF::AccessorType::VEC3 | GLTF::AccessorType::VEC4);
				case GLTF::PrimitiveAttribute::JOINTS: return GLTF::AccessorType::VEC4;
				case GLTF::PrimitiveAttribute::WEIGHT: return GLTF::AccessorType::VEC4;
			}
			VXM_CORE_ASSERT(false, "the attribute {0} is not valid.", attribute);
			return (GLTF::AccessorType)-1;
		}
		int Helper::GetComponentByteCount(GLTF::ComponentType componentType)
		{
			switch (componentType) {
				case GLTF::SignedByte: return 1;
				case GLTF::UnsignedByte: return 1;
				case GLTF::SignedShort: return 2;
				case GLTF::UnsignedShort: return 2;
					// case GLTF::SignedInt: return 4; // Deprecated ?
				case GLTF::UnsignedInt: return 4;
				case GLTF::Float: return 4;
				default: return 0;
			}
		}
		GLTF::PrimitiveAttribute Helper::GetPrimitiveAttribute(const std::string &attributeStr)
		{
			if(attributeStr == "POSITION") return GLTF::PrimitiveAttribute::POSITION;
			else if(attributeStr == "NORMAL") return GLTF::PrimitiveAttribute::NORMAL;
			else if(attributeStr == "TANGENT") return GLTF::PrimitiveAttribute::TANGENT;
			else if(attributeStr.starts_with("TEXCOORD")) return GLTF::PrimitiveAttribute::TEXCOORD;
			else if(attributeStr.starts_with("COLOR")) return GLTF::PrimitiveAttribute::COLOR;
			else if(attributeStr.starts_with("JOINTS")) return GLTF::PrimitiveAttribute::JOINTS;
			else if(attributeStr.starts_with("WEIGHT")) return GLTF::PrimitiveAttribute::WEIGHT;

			VXM_CORE_ASSERT(false, "The attribute '{0}' is not handled.", attributeStr);
			return (GLTF::PrimitiveAttribute)-1;
		}
	}// namespace Core
}// namespace Voxymore