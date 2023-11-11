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
	Ref<Model> Model::CreateModel(const Path &path, const Ref<Shader> &shader)
	{
		return CreateRef<Model>(path, shader);
	}

	template<typename T, GLTF::ComponentType ct, GLTF::AccessorType at>
	std::pair<const T *, size_t> GetBuffer(const std::string &attribute, tinygltf::Model &model, tinygltf::Primitive &primitive)
	{
		VXM_PROFILE_SCOPE("Model::Model -> Create Position Buffer");
		size_t sizeofValue = sizeof(T);
		int value = primitive.attributes[attribute];
		auto &accessor = model.accessors[value];
		auto &bufferView = model.bufferViews[accessor.bufferView];
		auto &buffer = model.buffers[bufferView.buffer];
		VXM_CORE_ASSERT(accessor.componentType == ct, "The component type should be {0} and it is {1}.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType) ct), GLTF::Helper::ComponentTypeToString((GLTF::ComponentType) accessor.componentType));
		VXM_CORE_ASSERT(accessor.type == at, "The component type should be {0} and it is {1}.", GLTF::Helper::AccessorTypeToString((GLTF::AccessorType) at), GLTF::Helper::AccessorTypeToString((GLTF::AccessorType) accessor.type));
		VXM_CORE_ASSERT(bufferView.byteLength >= accessor.count * sizeofValue, "byteLength '{0}' is not long enought.", bufferView.byteLength);
		size_t bufferItemsCount = bufferView.byteLength / sizeofValue;
		const T *bufferPtr = static_cast<const T *>(static_cast<const void *>(&buffer.data.at(0) + bufferView.byteOffset));
		return {bufferPtr, bufferItemsCount};
	}

	std::pair<const void *, size_t> GetRawBuffer(int value, tinygltf::Model &model, tinygltf::Primitive &primitive)
	{
		VXM_PROFILE_SCOPE("Model::Model -> GetRawBuffer");
		auto &accessor = model.accessors[value];
		auto &bufferView = model.bufferViews[accessor.bufferView];
		auto &buffer = model.buffers[bufferView.buffer];
		const void *bufferPtr = static_cast<const void *>(&buffer.data.at(0) + bufferView.byteOffset);
		return {bufferPtr, bufferView.byteLength};
	}

	glm::vec4 Convertu8Vec3ToVec4(const glm::u8vec3 &v) { return glm::vec4(glm::vec3(v) / 255.0f, 1.0f); }
	glm::vec4 Convertu16Vec3ToVec4(const glm::u16vec3 &v) { return glm::vec4(glm::vec3(v) / 65535.0f, 1.0f); }
	glm::vec4 Convertu32Vec3ToVec4(const glm::u32vec3 &v) { return glm::vec4(glm::vec3(v) / 4294967295.0f, 1.0f); }
	glm::vec4 ConvertVec3ToVec4(const glm::vec3 &v) { return glm::vec4(v, 1.0f); }

	glm::vec4 GetColor(const std::string &attribute, tinygltf::Model &model, tinygltf::Primitive &primitive, size_t verticesCount, size_t index)
	{
		glm::vec4 color;
		int colorValue = primitive.attributes[attribute];
		auto &colorAccessor = model.accessors[colorValue];
		auto &bufferView = model.bufferViews[colorAccessor.bufferView];
		auto &buffer = model.buffers[bufferView.buffer];
		const void *colorBuffer = static_cast<const void *>(&buffer.data.at(0) + bufferView.byteOffset);
		size_t colorSize = bufferView.byteLength;

		VXM_CORE_ASSERT(index >= 0 && index < verticesCount, "The index {0} is not valid.", index);

		if (colorAccessor.type == GLTF::AccessorType::VEC4) {
			switch ((GLTF::ComponentType) colorAccessor.componentType) {
				case GLTF::ComponentType::UnsignedByte:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u8vec4), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::u8vec4), verticesCount);
					color = (static_cast<const glm::u8vec4 *>(colorBuffer)[index]);
					color /= glm::vec4(255.0);
					break;
				case GLTF::ComponentType::UnsignedShort:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u16vec4), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::u16vec4), verticesCount);
					color = (static_cast<const glm::u16vec4 *>(colorBuffer)[index]);
					color /= glm::vec4(65535.0);
					break;
				case GLTF::ComponentType::UnsignedInt:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u32vec4), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::u32vec4), verticesCount);
					color = (static_cast<const glm::u32vec4 *>(colorBuffer)[index]);
					color /= glm::vec4(4294967295.0);
					break;
				case GLTF::ComponentType::Float:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::vec4), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::vec4), verticesCount);
					color = (static_cast<const glm::vec4 *>(colorBuffer)[index]);
					break;
				default:
					VXM_CORE_ASSERT(false, "The type {0} is not supported.", colorAccessor.componentType);
			}
		}
		else if (colorAccessor.type == GLTF::AccessorType::VEC3) {
			switch ((GLTF::ComponentType) colorAccessor.componentType) {
				case GLTF::ComponentType::UnsignedByte:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u8vec3), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::u8vec3), verticesCount);
					color = Convertu8Vec3ToVec4(static_cast<const glm::u8vec3 *>(colorBuffer)[index]);
				case GLTF::ComponentType::UnsignedShort:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u16vec3), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::u16vec3), verticesCount);
					color = Convertu16Vec3ToVec4(static_cast<const glm::u16vec3 *>(colorBuffer)[index]);
				case GLTF::ComponentType::UnsignedInt:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u32vec3), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::u32vec3), verticesCount);
					color = Convertu32Vec3ToVec4(static_cast<const glm::u32vec3 *>(colorBuffer)[index]);
				case GLTF::ComponentType::Float:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::vec3), "The Color Buffer only has {0} items instead of {1}", colorSize / sizeof(glm::vec3), verticesCount);
					color = ConvertVec3ToVec4(static_cast<const glm::vec3 *>(colorBuffer)[index]);
				default: VXM_CORE_ASSERT(false, "The type {0} is not supported.", colorAccessor.componentType);
			}
		}

		return color;
	}

	Model::Model(const Path &p, const Ref<Shader> &shader)
		: m_Path(p)
		, m_Shader(shader)
	{
		VXM_PROFILE_FUNCTION();

		auto path = p.GetFullPath();
        VXM_CORE_ASSERT(std::filesystem::exists(path), "The file {0} doesn't exist", path.string());

        //TODO: replace this with a real UUID of the model (that should be store somewhere I don't know).
        uint64_t modelId = std::hash<Path>()(p);

		UnflipStbi();
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool loaded = false;
		if (path.extension() == EXTENSION_GLTF) loaded = loader.LoadASCIIFromFile(&model, &err, &warn, path.string().c_str());
		else loaded = loader.LoadBinaryFromFile(&model, &err, &warn, path.string().c_str());// for binary glTF(.glb)

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

		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Meshes");
			m_Meshes.reserve(model.meshes.size());
			for (auto &mesh: model.meshes) {
				VXM_PROFILE_SCOPE("Model::Model -> Create MeshGroup");
				MeshGroup meshGroup;

				meshGroup.m_Meshes.reserve(mesh.primitives.size());

				for (auto &primitive: mesh.primitives) {
					VXM_PROFILE_SCOPE("Model::Model -> Create Mesh");

					std::vector<Vertex> vertexes;
					std::vector<uint32_t> index;

					{
						VXM_PROFILE_SCOPE("Model::Model -> Create Vertex Buffer");
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

						auto &&[positionsBuffer, positionsCount] = GetBuffer<glm::vec3, GLTF::ComponentType::Float, GLTF::AccessorType::VEC3>(positionAttribute, model, primitive);
						auto &&[normalsBuffer, normalsCount] = GetBuffer<glm::vec3, GLTF::ComponentType::Float, GLTF::AccessorType::VEC3>(normalAttribute, model, primitive);
						auto &&[texcoordsBuffer, texcoordsCount] = GetBuffer<glm::vec2, GLTF::ComponentType::Float, GLTF::AccessorType::VEC2>(texcoordAttribute, model, primitive);

						VXM_CORE_ASSERT(positionsCount == normalsCount && normalsCount == texcoordsCount, "The buffer are not the same size");
						size_t verticesCount = positionsCount;

						vertexes.reserve(verticesCount);

						if (primitive.attributes.contains(colorAttribute)) {
							for (size_t i = 0; i < verticesCount; i++) {
								vertexes.emplace_back(positionsBuffer[i], normalsBuffer[i], texcoordsBuffer[i], GetColor(colorAttribute, model, primitive, verticesCount, i));
							}
						}
						else {
							for (size_t i = 0; i < positionsCount; i++) {
								vertexes.emplace_back(positionsBuffer[i], normalsBuffer[i], texcoordsBuffer[i]);
							}
						}
					}

					// Index Buffer
					{
						VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer");
						auto &accessor = model.accessors[primitive.indices];
						auto &&[indexBuffer, indexSize] = GetRawBuffer(primitive.indices, model, primitive);
						switch ((GLTF::ComponentType) accessor.componentType) {
							case GLTF::ComponentType::UnsignedByte: {
								VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer -> UnsignedByte");
								size_t sizeofValue = sizeof(uint8_t);
								size_t bufferItemsCount = indexSize / sizeofValue;
								const auto *bufferPtr = static_cast<const uint8_t *>(indexBuffer);
								index.resize(bufferItemsCount);
								for (int i = 0; i < bufferItemsCount; ++i) {
									index[i] = bufferPtr[i];
								}
								break;
							}
							case GLTF::ComponentType::UnsignedShort: {
								VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer -> UnsignedShort");
								size_t sizeofValue = sizeof(uint16_t);
								size_t bufferItemsCount = indexSize / sizeofValue;
								const auto *bufferPtr = static_cast<const uint16_t *>(indexBuffer);
								index.resize(bufferItemsCount);
								for (int i = 0; i < bufferItemsCount; ++i) {
									index[i] = bufferPtr[i];
								}
								break;
							}
							case GLTF::ComponentType::UnsignedInt: {
								VXM_PROFILE_SCOPE("Model::Model -> Create Index Buffer -> UnsignedInt");
								size_t sizeofValue = sizeof(uint32_t);
								size_t bufferItemsCount = indexSize / sizeofValue;
								const auto *bufferPtr = static_cast<const uint32_t *>(indexBuffer);
								index.insert(index.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount]);
								break;
							}
							default: VXM_CORE_ASSERT(false, "The component type {0} is not supported for an index buffer.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType) accessor.componentType));
						}
					}

					Ref<Material> material;

					// Materials
					{
						VXM_PROFILE_SCOPE("Model::Model -> Create Material");
						if (primitive.material > -1) {
							tinygltf::Material mat = model.materials[primitive.material];

                            std::string matName = std::to_string(modelId) + "_" + mat.name;

							if (!MaterialLibrary::GetInstance().Exists(matName)) {
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
								material->SetMaterialName(matName);
								MaterialLibrary::GetInstance().Add(material);
							}
							else {
								material = MaterialLibrary::GetInstance().Get(matName);
							}
						}
						else {
							material = MaterialLibrary::GetInstance().GetOrCreate("Default", m_Shader->GetName());
						}
					}

					meshGroup.AddSubMesh(vertexes, index, material);
				}
				m_Meshes.push_back(meshGroup);
			}
		}

		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Nodes");
			m_Nodes.reserve(model.nodes.size());
			for (auto &node: model.nodes) {
				glm::mat4 matrix = GLTF::Helper::GetMatrix(node);
				std::vector<int> children = node.children;
				if (node.mesh > -1) m_Nodes.emplace_back(node.mesh, children, matrix);
				else m_Nodes.push_back(Node(children, matrix));
			}
		}

		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Textures");
			m_Textures.reserve(model.textures.size());
			for (auto &texture: model.textures) {
				VXM_PROFILE_SCOPE("Model::Model -> Create Texture");
				//TODO: remove the assert and leave a blank or magenta texture by default.
				VXM_CORE_ASSERT(texture.source > -1, "No texture associated.");
				auto &image = model.images[texture.source];
				if (image.bufferView > -1) {
					VXM_CORE_ASSERT(image.width > -1 && image.height > -1 && image.component > -1, "Cannot handle image if we don't have the size or the bits of each pixels.");
					VXM_CORE_ASSERT(image.image.size() != 0, "Cannot handle the current image...");
					int width = image.width;
					int height = image.height;
					int channels = image.component;
					if (image.bits == 8) {
						VXM_CORE_ASSERT(image.image.size() >= width * height * channels * (image.bits / 8), "The buffer is not long enought...");
						const auto *bufferPtr = static_cast<const uint8_t *>(image.image.data());
						m_Textures.push_back(Texture2D::Create(bufferPtr, width, height, channels));
					}
					else if (image.bits == 16) {
						VXM_CORE_ASSERT(image.image.size() >= width * height * channels * (image.bits / 8), "The buffer is not long enought...");
						const auto *bufferPtr = static_cast<const uint16_t *>(static_cast<const void *>(image.image.data()));
						m_Textures.push_back(Texture2D::Create(bufferPtr, width, height, channels));
					}
					else {
						VXM_CORE_ASSERT(false, "The pixel type {0} is not handled.", image.pixel_type);
					}
				}
				else {
					VXM_CORE_ASSERT(!image.uri.empty(), "The image don't have any way to be fetch.");
					VXM_CORE_ASSERT(!image.uri.starts_with("http"), "The engine cannot fetch the image from the internet for now.");
					VXM_CORE_ASSERT(image.mimeType == "image/jpeg" || image.mimeType == "image/png" || image.mimeType == "image/bmp" || image.mimeType == "image/gif", "Cannot handle the image type {0}.", image.mimeType);
					auto parentFolder = p.GetFullPath().parent_path();
					m_Textures.push_back(Texture2D::Create(parentFolder / image.uri));
				}
			}
		}

		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Scenes");
			m_Scenes.reserve(model.scenes.size());
			for (auto &scene: model.scenes) {
				m_Scenes.push_back(scene.nodes);
			}
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

	const Path& Model::GetPath() const
	{
		return m_Path;
	}

	const Ref<Shader>& Model::GetShader() const
	{
		return m_Shader;
	}

	void Model::SetShader(Ref<Shader> & shader)
	{
		m_Shader = shader;
		for (auto& meshGroup : m_Meshes)
		{
			for(auto& mesh : meshGroup.m_Meshes)
			{
				mesh.SetShader(shader);
			}
		}
	}

	void Model::SetShader(const std::string & shaderName)
	{
		m_Shader = ShaderLibrary::GetInstance().Get(shaderName);
		for (auto& meshGroup : m_Meshes)
		{
			for(auto& mesh : meshGroup.m_Meshes)
			{
				mesh.SetShader(shaderName);
			}
		}
	}

	const MeshGroup& Model::GetMeshGroup(int index) const
	{
		VXM_CORE_ASSERT(index > -1 && index < m_Meshes.size(), "The index '{0}' is not a valid mesh.", index);
		return m_Meshes[index];
	}
} // namespace Voxymore::Core
