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
#include <utility>

namespace Voxymore::Core
{
	Ref<Model> Model::CreateModel(const Path &path, const Ref<Shader>& shader)
	{
		return CreateRef<Model>(path, shader);
	}

	template<typename ValueT, typename BufferT, GLTF::ComponentType ct, GLTF::AccessorType at>
	void FillBuffer(std::vector<ValueT>& values, ValueT MaxValueBuffer, const std::string& attribute, tinygltf::Model& model, tinygltf::Primitive& primitive)
	{
		VXM_PROFILE_SCOPE("Model::Model -> Create Position Buffer");
		size_t sizeofValue = sizeof(ValueT);
		size_t sizeofBuffer = sizeof(BufferT);
		int value = primitive.attributes[attribute];
		auto& accessor = model.accessors[value];
		auto& bufferView = model.bufferViews[accessor.bufferView];
		auto& buffer = model.buffers[bufferView.buffer];
		VXM_CORE_ASSERT(accessor.componentType == ct, "The component type should be {0} and it is {1}.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)ct), GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
		VXM_CORE_ASSERT(accessor.type == at, "The component type should be {0} and it is {1}.", GLTF::Helper::AccessorTypeToString((GLTF::AccessorType)at), GLTF::Helper::AccessorTypeToString((GLTF::AccessorType)accessor.type));
		VXM_CORE_ASSERT(bufferView.byteLength >= accessor.count * sizeofBuffer, "byteLength {0} is not long enought {1}.", bufferView.byteLength, accessor.count * sizeofValue);
		size_t bufferItemsCount = bufferView.byteLength / sizeofBuffer;
		const auto*bufferPtr = static_cast<const BufferT*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
		values.resize(bufferItemsCount);
		for (int i = 0; i < bufferItemsCount; ++i) {
			values[i] = static_cast<ValueT>(bufferPtr[i]) / MaxValueBuffer;
		}
	}

	template<typename ValueT, typename BufferT, GLTF::ComponentType ct, GLTF::AccessorType at, typename ConvertT>
	void FillAndConvertBuffer(std::vector<ValueT>& values, const std::string& attribute, tinygltf::Model& model, tinygltf::Primitive& primitive, ConvertT converter)
	{
		VXM_PROFILE_SCOPE("Model::Model -> Create Position Buffer");
		size_t sizeofValue = sizeof(ValueT);
		size_t sizeofBuffer = sizeof(BufferT);
		int value = primitive.attributes[attribute];
		auto& accessor = model.accessors[value];
		auto& bufferView = model.bufferViews[accessor.bufferView];
		auto& buffer = model.buffers[bufferView.buffer];
		VXM_CORE_ASSERT(accessor.componentType == ct, "The component type should be {0} and it is {1}.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)ct), GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
		VXM_CORE_ASSERT(accessor.type == at, "The component type should be {0} and it is {1}.", GLTF::Helper::AccessorTypeToString((GLTF::AccessorType)at), GLTF::Helper::AccessorTypeToString((GLTF::AccessorType)accessor.type));
		VXM_CORE_ASSERT(bufferView.byteLength >= accessor.count * sizeofBuffer, "byteLength {0} is not long enought {1}.", bufferView.byteLength, accessor.count * sizeofValue);
		size_t bufferItemsCount = bufferView.byteLength / sizeofBuffer;
		const auto*bufferPtr = static_cast<const BufferT*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
		values.resize(bufferItemsCount);
		for (int i = 0; i < bufferItemsCount; ++i) {
			values[i] = converter(bufferPtr[i]);
		}
	}

	template<typename T, GLTF::ComponentType ct, GLTF::AccessorType at>
	void InsertBuffer(std::vector<T>& values, const std::string& attribute, tinygltf::Model& model, tinygltf::Primitive& primitive)
	{
		VXM_PROFILE_SCOPE("Model::Model -> Create Position Buffer");
		size_t sizeofValue = sizeof(T);
		int value = primitive.attributes[attribute];
		auto& accessor = model.accessors[value];
		auto& bufferView = model.bufferViews[accessor.bufferView];
		auto& buffer = model.buffers[bufferView.buffer];
		VXM_CORE_ASSERT(accessor.componentType == ct, "The component type should be {0} and it is {1}.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)ct), GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
		VXM_CORE_ASSERT(accessor.type == at, "The component type should be {0} and it is {1}.", GLTF::Helper::AccessorTypeToString((GLTF::AccessorType)at), GLTF::Helper::AccessorTypeToString((GLTF::AccessorType)accessor.type));
		VXM_CORE_ASSERT(bufferView.byteLength >= accessor.count * sizeofValue, "byteLength '{0}' is not long enought.", bufferView.byteLength);
		size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
		const auto*bufferPtr = static_cast<const T*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
		values.insert(values.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount]);
	}

	glm::vec4 Convertu8Vec3ToVec4(const glm::u8vec3& v) {return glm::vec4(glm::vec3(v) / 255.0f, 1.0f);}
	glm::vec4 Convertu16Vec3ToVec4(const glm::u16vec3& v) {return glm::vec4(glm::vec3(v) / 65535.0f, 1.0f);}
	glm::vec4 Convertu32Vec3ToVec4(const glm::u32vec3& v) {return glm::vec4(glm::vec3(v) / 4294967295.0f, 1.0f);}
	glm::vec4 ConvertVec3ToVec4(const glm::vec3& v) {return glm::vec4(v, 1.0f);}

	Model::Model(const Path &p, const Ref<Shader>& shader) : m_Path(p), m_Shader(shader)
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
				std::string colorAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::COLOR);

				VXM_CORE_ASSERT(primitive.attributes.contains(positionAttribute), "A primitive must possess Positions.");
				VXM_CORE_ASSERT(primitive.attributes.contains(normalAttribute), "A primitive must possess Normals.");
				VXM_CORE_ASSERT(primitive.attributes.contains(texcoordAttribute), "A primitive must possess Texcoords.");

				//Safeguards
				VXM_CORE_ASSERT(sizeof(glm::vec3) == sizeof(float) * 3, "glm::vec3 is not equal to 3 floats...");
				VXM_CORE_ASSERT(sizeof(glm::vec2) == sizeof(float) * 2, "glm::vec2 is not equal to 2 floats...");

				std::vector<glm::vec3> positions;
				std::vector<glm::vec3> normals;
				std::vector<glm::vec2> texcoords;
				std::vector<glm::vec4> colors;
				std::vector<uint32_t> index;

				InsertBuffer<glm::vec3, GLTF::ComponentType::Float, GLTF::AccessorType::VEC3>(positions, positionAttribute, model, primitive);

				InsertBuffer<glm::vec3, GLTF::ComponentType::Float, GLTF::AccessorType::VEC3>(normals, normalAttribute, model, primitive);

				InsertBuffer<glm::vec2, GLTF::ComponentType::Float, GLTF::AccessorType::VEC2>(texcoords, texcoordAttribute, model, primitive);

				if(primitive.attributes.contains(colorAttribute))
				{
					VXM_PROFILE_SCOPE("Model::Model -> Create Color Buffer");
					int value = primitive.attributes[colorAttribute];
					auto& accessor = model.accessors[value];
					auto& bufferView = model.bufferViews[accessor.bufferView];
					auto& buffer = model.buffers[bufferView.buffer];
					if(accessor.type == GLTF::AccessorType::VEC4 && accessor.componentType == GLTF::ComponentType::Float)
					{
						InsertBuffer<glm::vec4, GLTF::ComponentType::Float, GLTF::AccessorType::VEC4>(colors, colorAttribute, model, primitive);
					}
					else
					{
						colors.resize(positions.size(), glm::vec4(1.0f));
						if(accessor.type == GLTF::AccessorType::VEC4)
						{
							switch ((GLTF::ComponentType)accessor.componentType) {
								case GLTF::ComponentType::UnsignedByte:
									FillBuffer<glm::vec4, glm::u8vec4, GLTF::ComponentType::UnsignedByte, GLTF::AccessorType::VEC4>(colors, glm::vec4(255.0), colorAttribute, model, primitive); break;
								case GLTF::ComponentType::UnsignedShort:
									FillBuffer<glm::vec4, glm::u16vec4, GLTF::ComponentType::UnsignedShort, GLTF::AccessorType::VEC4>(colors, glm::vec4(65535.0), colorAttribute, model, primitive); break;
								case GLTF::ComponentType::UnsignedInt:
									FillBuffer<glm::vec4, glm::u32vec4, GLTF::ComponentType::UnsignedInt, GLTF::AccessorType::VEC4>(colors, glm::vec4(4294967295.0), colorAttribute, model, primitive); break;
								default:
									VXM_CORE_ASSERT(false, "The type {0} is not supported.", accessor.componentType);
							}
						}
						else if(accessor.type == GLTF::AccessorType::VEC3)
						{
							switch ((GLTF::ComponentType)accessor.componentType) {
								case GLTF::ComponentType::UnsignedByte: FillAndConvertBuffer<glm::vec4, glm::u8vec3, GLTF::ComponentType::UnsignedByte, GLTF::AccessorType::VEC3>(colors, colorAttribute, model, primitive, Convertu8Vec3ToVec4); break;
								case GLTF::ComponentType::UnsignedShort: FillAndConvertBuffer<glm::vec4, glm::u16vec3, GLTF::ComponentType::UnsignedShort, GLTF::AccessorType::VEC3>(colors, colorAttribute, model, primitive, Convertu16Vec3ToVec4); break;
								case GLTF::ComponentType::UnsignedInt: FillAndConvertBuffer<glm::vec4, glm::u32vec3, GLTF::ComponentType::UnsignedInt, GLTF::AccessorType::VEC3>(colors, colorAttribute, model, primitive, Convertu32Vec3ToVec4); break;
								case GLTF::ComponentType::Float: FillAndConvertBuffer<glm::vec4, glm::vec3, GLTF::ComponentType::Float, GLTF::AccessorType::VEC3>(colors, colorAttribute, model, primitive, ConvertVec3ToVec4); break;
								default: VXM_CORE_ASSERT(false, "The type {0} is not supported.", accessor.componentType);
							}
						}
					}
				}
				else
				{
					VXM_PROFILE_SCOPE("Model::Model -> Create Color Buffer");
					colors.resize(positions.size(), glm::vec4(1.0f));
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
							VXM_CORE_ASSERT(bufferView.byteLength >= accessor.count * sizeofValue, "byteLength {0} is not long enought {1}.", bufferView.byteLength, accessor.count * sizeofValue);
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
							VXM_CORE_ASSERT(bufferView.byteLength >= accessor.count * sizeofValue, "byteLength {0} is not long enought {1}.", bufferView.byteLength, accessor.count * sizeofValue);
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
							VXM_CORE_ASSERT(bufferView.byteLength >= accessor.count * sizeofValue, "byteLength {0} is not long enought {1}.", bufferView.byteLength, accessor.count * sizeofValue);
							size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
							const auto*bufferPtr = static_cast<const uint32_t*>(static_cast<const void*>(&buffer.data.at(0) + bufferView.byteOffset));
							index.insert(index.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount]);
							break;
						}
						default: VXM_CORE_ASSERT(false, "The component type {0} is not supported for an index buffer.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType)accessor.componentType));
					}
				}

				Ref<Material> material;

				// Materials
				{
					if(primitive.material > -1)
					{
						tinygltf::Material mat = model.materials[primitive.material];
						if(!MaterialLibrary::GetInstance().Exists(mat.name))
						{
							MaterialParameters materialParams;

							// PbrMetallicRoughness
							materialParams.PbrMetallicRoughness.BaseColorFactor = glm::vec4(mat.pbrMetallicRoughness.baseColorFactor[0], mat.pbrMetallicRoughness.baseColorFactor[1], mat.pbrMetallicRoughness.baseColorFactor[2], mat.pbrMetallicRoughness.baseColorFactor[3]);
							materialParams.PbrMetallicRoughness.BaseColorTexture.Index = mat.pbrMetallicRoughness.baseColorTexture.index;
							materialParams.PbrMetallicRoughness.BaseColorTexture.TexCoord = mat.pbrMetallicRoughness.baseColorTexture.texCoord;

							materialParams.PbrMetallicRoughness.MetallicFactor = mat.pbrMetallicRoughness.metallicFactor;
							materialParams.PbrMetallicRoughness.RoughnessFactor = mat.pbrMetallicRoughness.roughnessFactor;
							materialParams.PbrMetallicRoughness.MetallicRoughnessTexture.Index = mat.pbrMetallicRoughness.metallicRoughnessTexture.index;
							materialParams.PbrMetallicRoughness.MetallicRoughnessTexture.TexCoord = mat.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;


							// Normal Texture
							materialParams.NormalTexture.Index = mat.normalTexture.index;
							materialParams.NormalTexture.TexCoord = mat.normalTexture.texCoord;
							materialParams.NormalTexture.Scale = mat.normalTexture.scale;


							// Occlusion Texture
							materialParams.OcclusionTexture.Index = mat.occlusionTexture.index;
							materialParams.OcclusionTexture.TexCoord = mat.occlusionTexture.texCoord;
							materialParams.OcclusionTexture.Strength = mat.occlusionTexture.strength;


							// Emissive Texture
							materialParams.EmissiveTexture.Index = mat.occlusionTexture.index;
							materialParams.EmissiveTexture.TexCoord = mat.occlusionTexture.texCoord;


							// classical parameters
							materialParams.EmissiveFactor = {mat.emissiveFactor[0], mat.emissiveFactor[1], mat.emissiveFactor[2]};
							//							materialParams.SetAlphaMode(mat.alphaMode);
							materialParams.AlphaCutoff = mat.alphaCutoff;
							materialParams.DoubleSided = mat.doubleSided;

							material = CreateRef<Material>(m_Shader, materialParams);
							material->SetMaterialName(mat.name);
							MaterialLibrary::GetInstance().Add(material);
						}
						else
						{
							material = MaterialLibrary::GetInstance().Get(mat.name);
						}
					}
					else
					{
						material = MaterialLibrary::GetInstance().GetOrCreate("Default", m_Shader->GetName());
					}
				}

				m->AddSubMesh(positions, normals, texcoords, colors, index, material);
			}
			m_Meshes.push_back(m);
		}

		m_Nodes.reserve(model.nodes.size());
		for(auto& node : model.nodes)
		{
			glm::mat4 matrix = GLTF::Helper::GetMatrix(node);
			std::vector<int> children = node.children;
			if(node.mesh > -1) m_Nodes.push_back(Node(m_Meshes[node.mesh], children, matrix));
			else m_Nodes.push_back(Node(children, matrix));
		}

		m_Textures.reserve(model.textures.size());
		for (auto& texture : model.textures)
		{
			//TODO: remove the assert and leave a blank or magenta texture by default.
			VXM_CORE_ASSERT(texture.source > -1, "No texture associated.");
			auto& image = model.images[texture.source];
			if(image.bufferView > -1)
			{
				VXM_CORE_ASSERT(image.width > -1 && image.height > -1 && image.component > -1, "Cannot handle image if we don't have the size or the bits of each pixels.");
				VXM_CORE_ASSERT(image.image.size() != 0, "Cannot handle the current image...");
				int width = image.width;
				int height = image.height;
				int channels = image.component;
				if(image.bits == 8)
				{
					VXM_CORE_ASSERT(image.image.size()>= width * height * channels * (image.bits/8), "The buffer is not long enought...");
					const auto*bufferPtr = static_cast<const uint8_t*>(image.image.data());
					m_Textures.push_back(Texture2D::Create(bufferPtr, width, height, channels));
				}
				else if(image.bits == 16)
				{
					VXM_CORE_ASSERT(image.image.size()>= width * height * channels * (image.bits/8), "The buffer is not long enought...");
					const auto*bufferPtr = static_cast<const uint16_t*>(static_cast<const void*>(image.image.data()));
					m_Textures.push_back(Texture2D::Create(bufferPtr, width, height, channels));
				}
				else
				{
					VXM_CORE_ASSERT(false, "The pixel type {0} is not handled.", image.pixel_type);
				}
			}
			else
			{
				VXM_CORE_ASSERT(!image.uri.empty(), "The image don't have any way to be fetch.");
				VXM_CORE_ASSERT(!image.uri.starts_with("http"), "The engine cannot fetch the image from the internet for now.");
				VXM_CORE_ASSERT(image.mimeType == "image/jpeg" || image.mimeType == "image/png" || image.mimeType == "image/bmp" || image.mimeType == "image/gif", "Cannot handle the image type {0}.", image.mimeType);
				auto parentFolder = p.GetFullPath().parent_path();
				m_Textures.push_back(Texture2D::Create(parentFolder / image.uri));
			}
		}

		m_Scenes.reserve(model.scenes.size());

		for (auto& scene : model.scenes)
		{
			m_Scenes.push_back(scene.nodes);
		}
	}

	Model::~Model()
	{
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

	void Model::Bind()
	{
		m_Shader->Bind();
		for (int i = 0; i < m_Textures.size(); ++i)
		{
			m_Textures[i]->Bind(i);
		}
	}
	void Model::Unbind()
	{
		m_Shader->Unbind();
	}
} // namespace Voxymore::Core
