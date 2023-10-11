//
// Created by Sayama on 27/09/2023.
//

#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Math.hpp"

// PRIVATE USE ONLY
#include "GLTFHelper.hpp"
#include <tiny_gltf.h>
#include <unordered_map>

namespace Voxymore::Core
{
	namespace Utils
	{
		bool IsValid(int value) {return value > -1;}
	}

	Ref<Model> Model::CreateModel(const Path &path)
	{
		return CreateRef<Model>(path);
	}

	Model::Model(const Path &p) : m_Path(p)
	{
		VXM_PROFILE_FUNCTION();
		auto path = p.GetFullPath();
		VXM_CORE_ASSERT(std::filesystem::exists(path), "The file {0} doesn't exist", path.string());
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool loaded = false;
		if(path.extension() == EXTENSION_GLTF) loaded = loader.LoadASCIIFromFile(&model, &err, &warn, path.string().c_str());
		else loaded = loader.LoadBinaryFromFile(&model, &err, &warn, path.string().c_str()); // for binary glTF(.glb)

		if (!warn.empty()) {
			VXM_CORE_WARNING(warn);
		}

		if (!err.empty()) {
			VXM_CORE_ERROR(err);
		}

		if (!loaded) {
			VXM_CORE_ERROR("Failed to parse glTF {0}", path.string());
			return;
		}

		m_Meshes.reserve(model.meshes.size());
		for (auto& mesh : model.meshes)
		{
			Ref<Mesh> m = CreateRef<Mesh>();
			// TODO: set the shader (or material ?) of the shader.
			m->m_SubMeshes.reserve(mesh.primitives.size());
			for (auto& primitive : mesh.primitives)
			{
				//TODO: Optimize this to be able to add the rest of the possible attributes. (at least multiple tex coords);
				//TODO2: Add other render mode.
				VXM_CORE_ASSERT(primitive.mode == GLTF::MeshRenderMode::TRIANGLES, "The Render Mode {0} cannot be used for the moment.", primitive.mode);
				VXM_CORE_ASSERT(primitive.indices > 0, "The indices are currently required to load a 3d model.");

				std::string positionAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::POSITION);
				std::string normalAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::NORMAL);
				std::string texcoordAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::TEXCOORD);

				VXM_CORE_ASSERT(primitive.attributes.contains(positionAttribute), "A primitive must possess Positions.");
				VXM_CORE_ASSERT(primitive.attributes.contains(normalAttribute), "A primitive must possess Normals.");
				VXM_CORE_ASSERT(primitive.attributes.contains(texcoordAttribute), "A primitive must possess Texcoords.");

				//Safeguards
				VXM_CORE_ASSERT(sizeof(glm::vec3) == sizeof(float) * 3, "glm::vec3 is not equal to 3 floats...");
				VXM_CORE_ASSERT(sizeof(glm::vec2) == sizeof(float) * 2, "glm::vec2 is not equal to 2 floats...");

				std::vector<glm::vec3> positions;
				std::vector<glm::vec3> normal;
				std::vector<glm::vec2> texcoords;
				std::vector<uint32_t> index;

				// Position Buffer
				{
					VXM_PROFILE_SCOPE("Model::Model -> Create Position Buffer");
					size_t sizeofValue = sizeof(glm::vec3);
					int value = primitive.attributes[positionAttribute];
					auto& accessor = model.accessors[value];
					auto& bufferView = model.bufferViews[accessor.bufferView];
					auto& buffer = model.buffers[bufferView.buffer];
					VXM_CORE_ASSERT(accessor.componentType == GLTF::ComponentType::Float, "The component type should be Float and it is {0}.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
					VXM_CORE_ASSERT(bufferView.byteLength % sizeofValue == 0, "byteLength {0} is not correct.", bufferView.byteLength);
					size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
					const auto*bufferPtr = static_cast<const glm::vec3*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
					positions.insert(positions.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount - 1]);
				}

				// Normal Buffer
				{
					VXM_PROFILE_SCOPE("Model::Model -> Create Normal Buffer");
					size_t sizeofValue = sizeof(glm::vec3);
					int value = primitive.attributes[normalAttribute];
					auto& accessor = model.accessors[value];
					auto& bufferView = model.bufferViews[accessor.bufferView];
					auto& buffer = model.buffers[bufferView.buffer];
					VXM_CORE_ASSERT(accessor.componentType == GLTF::ComponentType::Float, "The component type should be Float and it is {0}.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
					VXM_CORE_ASSERT(bufferView.byteLength % sizeofValue == 0, "byteLength {0} is not correct.", bufferView.byteLength);
					size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
					const auto*bufferPtr = static_cast<const glm::vec3*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
					normal.insert(normal.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount - 1]);
				}

				// Texcoord Buffer
				{
					VXM_PROFILE_SCOPE("Model::Model -> Create Texcoord Buffer");
					size_t sizeofValue = sizeof(glm::vec2);
					int value = primitive.attributes[texcoordAttribute];
					auto& accessor = model.accessors[value];
					auto& bufferView = model.bufferViews[accessor.bufferView];
					auto& buffer = model.buffers[bufferView.buffer];
					VXM_CORE_ASSERT(accessor.componentType == GLTF::ComponentType::Float, "The component type should be Float and it is {0}.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
					VXM_CORE_ASSERT(bufferView.byteLength % sizeofValue == 0, "byteLength {0} is not correct.", bufferView.byteLength);
					size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
					const auto*bufferPtr = static_cast<const glm::vec2*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
					texcoords.insert(texcoords.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount - 1]);
				}

				// Index Buffer
				{
					VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer");
					int value = primitive.indices;
					auto& accessor = model.accessors[value];
					auto& bufferView = model.bufferViews[accessor.bufferView];
					auto& buffer = model.buffers[bufferView.buffer];
					switch((GLTF::ComponentType)accessor.componentType)
					{
						case GLTF::ComponentType::UnsignedByte:
						{
							VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer -> UnsignedByte");
							size_t sizeofValue = sizeof(uint8_t);
							VXM_CORE_ASSERT(bufferView.byteLength % sizeofValue == 0, "byteLength {0} is not correct.", bufferView.byteLength);
							size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
							const auto*bufferPtr = static_cast<const uint8_t*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
							index.resize(bufferItemsCount);
							for (int i = 0; i < bufferItemsCount; ++i)
							{
								index[i] = bufferPtr[i];
							}
							break;
						}
						case GLTF::ComponentType::UnsignedShort:
						{
							VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer -> UnsignedShort");
							size_t sizeofValue = sizeof(uint16_t);
							VXM_CORE_ASSERT(bufferView.byteLength % sizeofValue == 0, "byteLength {0} is not correct.", bufferView.byteLength);
							size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
							const auto*bufferPtr = static_cast<const uint16_t*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
							index.resize(bufferItemsCount);
							for (int i = 0; i < bufferItemsCount; ++i)
							{
								index[i] = bufferPtr[i];
							}
							break;
						}
						case GLTF::ComponentType::UnsignedInt:
						{
							VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer -> UnsignedInt");
							size_t sizeofValue = sizeof(uint32_t);
							VXM_CORE_ASSERT(bufferView.byteLength % sizeofValue == 0, "byteLength {0} is not correct.", bufferView.byteLength);
							size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
							const auto*bufferPtr = static_cast<const uint32_t*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
							index.insert(index.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount - 1]);
							break;
						}
						default: VXM_CORE_ASSERT(false, "The component type {0} is not supported for an index buffer.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
					}
				}

				VXM_CORE_ASSERT(positions.size() == normal.size() == texcoords.size(), "The vertexe count are not the sames...");
				m->AddSubMesh(positions, normal, texcoords, index);
			}
			m_Meshes.push_back(m);
		}

		m_Nodes.reserve(model.nodes.size());
		for(auto& node : model.nodes)
		{
			m_Nodes.emplace_back(node.mesh > -1 ? m_Meshes[node.mesh] : nullptr, node.children, GLTF::Helper::GetMatrix(node));
		}

		m_Scenes.reserve(model.scenes.size());
		for (auto& scene : model.scenes)
		{
			m_Scenes.push_back(scene.nodes);
		}
	}

	const Node& Model::GetNode(int index) const
	{
		VXM_CORE_ASSERT(index >= 0 && index < m_Nodes.size(), "Index {0} is invalid.", index);
		return m_Nodes[index];
	}
	const std::vector<int> &Model::GetDefaultScene() const
	{
		VXM_CORE_ASSERT(!m_Scenes.empty(), "No scene on the model...");
		return  m_Scenes[m_DefaultScene];
	}
} // namespace Voxymore::Core
