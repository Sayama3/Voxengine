//
// Created by ianpo on 12/04/2024.
//

#include "Voxymore/Assets/Importers/MeshImporter.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Core/YamlHelper.hpp"

// PRIVATE USE ONLY
#include "Renderer/GLTFHelper.hpp"
#include <tiny_gltf.h>
#include <unordered_map>
#include <utility>
#include <stdexcept>

class NotImplemented : public std::logic_error
{
public:
	NotImplemented(std::string functionName) : std::logic_error(std::string("Function '") + functionName +"' not yet implemented") { };
	NotImplemented() : std::logic_error("Function not yet implemented") { };
};

namespace Voxymore::Core
{

	BoundingBox GetBoundingBox(const std::string &attribute, tinygltf::Model &model, tinygltf::Primitive &primitive)
	{
		VXM_PROFILE_FUNCTION();
		int value = primitive.attributes[attribute];
		auto &accessor = model.accessors[value];
		BoundingBox aabb;
		if(accessor.minValues.size() >= 3 && accessor.maxValues.size() >= 3) {
			aabb.SetMinMax(Vec3(accessor.minValues[0], accessor.minValues[1], accessor.minValues[2]),
						   Vec3(accessor.maxValues[0], accessor.maxValues[1], accessor.maxValues[2]));
		}
		return aabb;
	}

	template<typename T, GLTF::ComponentType ct, GLTF::AccessorType at>
	std::pair<const T *, size_t> GetBuffer(const std::string &attribute, tinygltf::Model &model, tinygltf::Primitive &primitive)
	{
		VXM_PROFILE_FUNCTION();
		//		VXM_PROFILE_SCOPE("Model::Model -> Create Position RendererBuffer");
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
		VXM_PROFILE_FUNCTION();
		//		VXM_PROFILE_SCOPE("Model::Model -> GetRawBuffer");
		auto &accessor = model.accessors[value];
		auto &bufferView = model.bufferViews[accessor.bufferView];
		auto &buffer = model.buffers[bufferView.buffer];
		const void *bufferPtr = static_cast<const void *>(&buffer.data.at(0) + bufferView.byteOffset);
		return {bufferPtr, bufferView.byteLength};
	}

	glm::vec4 Convertu8Vec3ToVec4(const glm::u8vec3 &v) { VXM_PROFILE_FUNCTION(); return glm::vec4(glm::vec3(v) / 255.0f, 1.0f); }
	glm::vec4 Convertu16Vec3ToVec4(const glm::u16vec3 &v) { VXM_PROFILE_FUNCTION(); return glm::vec4(glm::vec3(v) / 65535.0f, 1.0f); }
	glm::vec4 Convertu32Vec3ToVec4(const glm::u32vec3 &v) { VXM_PROFILE_FUNCTION(); return glm::vec4(glm::vec3(v) / 4294967295.0f, 1.0f); }
	glm::vec4 ConvertVec3ToVec4(const glm::vec3 &v) { VXM_PROFILE_FUNCTION(); return glm::vec4(v, 1.0f); }

	glm::vec4 GetColor(const std::string &attribute, tinygltf::Model &model, tinygltf::Primitive &primitive, size_t verticesCount, size_t index)
	{
		VXM_PROFILE_FUNCTION();
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
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u8vec4), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::u8vec4), verticesCount);
					color = (static_cast<const glm::u8vec4 *>(colorBuffer)[index]);
					color /= glm::vec4(255.0);
					break;
				case GLTF::ComponentType::UnsignedShort:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u16vec4), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::u16vec4), verticesCount);
					color = (static_cast<const glm::u16vec4 *>(colorBuffer)[index]);
					color /= glm::vec4(65535.0);
					break;
				case GLTF::ComponentType::UnsignedInt:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u32vec4), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::u32vec4), verticesCount);
					color = (static_cast<const glm::u32vec4 *>(colorBuffer)[index]);
					color /= glm::vec4(4294967295.0);
					break;
				case GLTF::ComponentType::Float:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::vec4), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::vec4), verticesCount);
					color = (static_cast<const glm::vec4 *>(colorBuffer)[index]);
					break;
				default:
					VXM_CORE_ASSERT(false, "The type {0} is not supported.", colorAccessor.componentType);
			}
		}
		else if (colorAccessor.type == GLTF::AccessorType::VEC3) {
			switch ((GLTF::ComponentType) colorAccessor.componentType) {
				case GLTF::ComponentType::UnsignedByte:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u8vec3), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::u8vec3), verticesCount);
					color = Convertu8Vec3ToVec4(static_cast<const glm::u8vec3 *>(colorBuffer)[index]);
				case GLTF::ComponentType::UnsignedShort:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u16vec3), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::u16vec3), verticesCount);
					color = Convertu16Vec3ToVec4(static_cast<const glm::u16vec3 *>(colorBuffer)[index]);
				case GLTF::ComponentType::UnsignedInt:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::u32vec3), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::u32vec3), verticesCount);
					color = Convertu32Vec3ToVec4(static_cast<const glm::u32vec3 *>(colorBuffer)[index]);
				case GLTF::ComponentType::Float:
					VXM_CORE_ASSERT(verticesCount == colorSize / sizeof(glm::vec3), "The Color RendererBuffer only has {0} items instead of {1}", colorSize / sizeof(glm::vec3), verticesCount);
					color = ConvertVec3ToVec4(static_cast<const glm::vec3 *>(colorBuffer)[index]);
				default: VXM_CORE_ASSERT(false, "The type {0} is not supported.", colorAccessor.componentType);
			}
		}

		return color;
	}


	bool MeshSerializer::IsMesh(const std::filesystem::path &path)
	{
		VXM_PROFILE_FUNCTION();
		std::string extension = path.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		return path.has_extension() && extension == MeshExtension;
	}

	Ref<Asset> MeshSerializer::ImportMesh(const AssetMetadata &metadata)
	{
		VXM_CORE_ASSERT(false, "Not implemented yet.");
		throw NotImplemented("MeshSerializer::ImportMesh");
	}

	void MeshSerializer::ExportEditorMesh(const AssetMetadata &metadata, Ref<Mesh> mesh)
	{
		VXM_CORE_ASSERT(false, "Not implemented yet.");
		throw NotImplemented("MeshSerializer::ExportEditorMesh");
	}

	bool MeshSerializer::IsModel(const std::filesystem::path& path)
	{
		VXM_PROFILE_FUNCTION();
		std::string extension = path.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		return path.has_extension() && std::find(ModelExtensions.begin(), ModelExtensions.end(), extension) != ModelExtensions.end();
	}

	ShaderField GetShader(const AssetMetadata& metadata)
	{
		VXM_PROFILE_FUNCTION();
		auto path = metadata.FilePath.GetFullPath();

		auto metaFilePath = metadata.FilePath;
		metaFilePath.path += ".meta";

		ShaderField shader = NullAssetHandle;

		if(FileSystem::Exist(metaFilePath))
		{
			YAML::Node root = FileSystem::ReadFileAsYAML(metaFilePath);
			YAML::Node shaderNode = root["Shader"];
			if (shaderNode)
			{
				shader = shaderNode.as<AssetHandle>();
			}
		}

		return shader;
	}

	Ref<Asset> MeshSerializer::ImportModel(const AssetMetadata& metadata)
	{
		VXM_CORE_ASSERT(IsModel(metadata.FilePath), "The asset '{0}' is not a 3d model", metadata.FilePath.string());

		auto path = metadata.FilePath.GetFullPath();
		VXM_CORE_ASSERT(std::filesystem::exists(path), "The file {0} doesn't exist", path.string());

		//TODO: replace this with a Real UUID of the model (that should be store somewhere I don't know).
		uint64_t modelId = std::hash<Path>()(metadata.FilePath);

		ShaderField shader = GetShader(metadata);

		UnflipStbi();
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool loaded = false;
		if (path.extension() == VXM_EXTENSION_GLTF) loaded = loader.LoadASCIIFromFile(&model, &err, &warn, path.string().c_str());
		else loaded = loader.LoadBinaryFromFile(&model, &err, &warn, path.string().c_str());// for binary glTF(.glb)

		if (!warn.empty()) {
			VXM_CORE_WARNING(warn);
		}

		if (!err.empty()) {
			VXM_CORE_ERROR(err);
		}

		if (!loaded) {
			VXM_CORE_ERROR("Failed to parse glTF {0}", path.string());
			return nullptr;
		}

		auto assetManager = Project::GetActive()->GetEditorAssetManager();
		Ref<::Voxymore::Core::Model> vxmModel = CreateRef<::Voxymore::Core::Model>(std::vector<MeshGroup>{}, std::vector<Node>{}, std::vector<ModelRootScene>{});

		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Nodes");
			vxmModel->m_Nodes.reserve(model.nodes.size());
			for (auto &node: model.nodes) {
				glm::mat4 matrix = GLTF::Helper::GetMatrix(node);
				std::vector<int> children = node.children;
				if (node.mesh > -1) vxmModel->m_Nodes.emplace_back(node.mesh, children, matrix);
				else
					vxmModel->m_Nodes.emplace_back(children, matrix);
			}
		}
		std::vector<Texture2DField> textures;
		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Textures");
			textures.reserve(model.textures.size());
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
						textures.push_back(Texture2D::Create(bufferPtr, width, height, channels));
					}
					else if (image.bits == 16) {
						VXM_CORE_ASSERT(image.image.size() >= width * height * channels * (image.bits / 8), "The buffer is not long enought...");
						const auto *bufferPtr = static_cast<const uint16_t *>(static_cast<const void *>(image.image.data()));
						textures.push_back(Texture2D::Create(bufferPtr, width, height, channels));
					}
					else {
						VXM_CORE_ASSERT(false, "The pixel type {0} is not handled.", image.pixel_type);
					}
				}
				else {
					VXM_CORE_ASSERT(!image.uri.empty(), "The image don't have any way to be fetch.");
					VXM_CORE_ASSERT(!image.uri.starts_with("http"), "The engine cannot fetch the image from the internet for now.");
					VXM_CORE_ASSERT(image.mimeType == "image/jpeg" || image.mimeType == "image/png" || image.mimeType == "image/bmp" || image.mimeType == "image/gif" || image.uri.ends_with(".png") || image.uri.ends_with(".jpg") || image.uri.ends_with(".jepg") || image.uri.ends_with(".bmp") || image.uri.ends_with(".gif"), "Cannot handle the image ({}) type {}.", image.uri, image.mimeType);
					auto parentFolder = metadata.FilePath.GetFullPath().parent_path();
					// TODO: Use the AssetManager
					Ref<Texture2D> asset = TextureImporter::LoadTexture2D(Path::GetPath(parentFolder / image.uri));
					assetManager->AddMemoryAsset(asset);
					textures.push_back(asset);
				}
			}
		}

		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Scenes");
			vxmModel->m_Scenes.reserve(model.scenes.size());
			for (auto &scene: model.scenes) {
				vxmModel->m_Scenes.push_back(scene.nodes);
			}
		}

		vxmModel->m_Materials.resize(model.materials.size());
		{
			VXM_PROFILE_SCOPE("Model::Model -> Create Meshes");
			vxmModel->m_Meshes.reserve(model.meshes.size());
			for (auto &mesh: model.meshes) {
				VXM_PROFILE_SCOPE("Model::Model -> Create MeshGroup");
				MeshGroup meshGroup;

				meshGroup.reserve(mesh.primitives.size());

				for (auto &primitive: mesh.primitives) {
					VXM_PROFILE_SCOPE("Model::Model -> Create Mesh");

					std::vector<Vertex> vertexes;
					std::vector<uint32_t> index;
					BoundingBox aabb;

					{
						VXM_PROFILE_SCOPE("Model::Model -> Create Vertex RendererBuffer");
						//TODO: Optimize this to be able to add the rest of the possible attributes. (at least multiple tex coords);
						//TODO2: Add other render mode.
						VXM_CORE_CHECK(primitive.mode == GLTF::MeshRenderMode::TRIANGLES, "The Render Mode {0} cannot be used for the moment.", primitive.mode);

						if(!(primitive.mode == GLTF::MeshRenderMode::TRIANGLES)) continue;

						std::string positionAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::POSITION);
						std::string normalAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::NORMAL);
						std::string texcoordAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::TEXCOORD);
						std::string colorAttribute = GLTF::Helper::GetPrimitiveAttributeString(GLTF::PrimitiveAttribute::COLOR);

						VXM_CORE_CHECK(primitive.attributes.contains(positionAttribute), "A primitive must possess Positions.");
						VXM_CORE_CHECK(primitive.attributes.contains(normalAttribute), "A primitive must possess Normals.");
						VXM_CORE_CHECK(primitive.attributes.contains(texcoordAttribute), "A primitive must possess Texcoords.");

						if(!primitive.attributes.contains(positionAttribute)) continue;
						if(!primitive.attributes.contains(normalAttribute)) continue;
						if(!primitive.attributes.contains(texcoordAttribute)) continue;

						//Safeguards
						VXM_CORE_ASSERT(sizeof(glm::vec3) == sizeof(float) * 3, "glm::vec3 is not equal to 3 floats...");
						VXM_CORE_ASSERT(sizeof(glm::vec2) == sizeof(float) * 2, "glm::vec2 is not equal to 2 floats...");

						auto &&[positionsBuffer, positionsCount] = GetBuffer<glm::vec3, GLTF::ComponentType::Float, GLTF::AccessorType::VEC3>(positionAttribute, model, primitive);
						auto &&[normalsBuffer, normalsCount] = GetBuffer<glm::vec3, GLTF::ComponentType::Float, GLTF::AccessorType::VEC3>(normalAttribute, model, primitive);
						auto &&[texcoordsBuffer, texcoordsCount] = GetBuffer<glm::vec2, GLTF::ComponentType::Float, GLTF::AccessorType::VEC2>(texcoordAttribute, model, primitive);
						aabb = GetBoundingBox(positionAttribute, model, primitive);

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

					// Index RendererBuffer
					{
						VXM_PROFILE_SCOPE("Model::Model -> Create Index RendererBuffer");
						if(primitive.indices > 0) {
							auto &accessor = model.accessors[primitive.indices];
							auto &&[indexBuffer, indexSize] = GetRawBuffer(primitive.indices, model, primitive);
							switch ((GLTF::ComponentType) accessor.componentType) {
								case GLTF::ComponentType::UnsignedByte: {
									VXM_PROFILE_SCOPE("Model::Model -> Create Index RendererBuffer -> UnsignedByte");
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
									VXM_PROFILE_SCOPE("Model::Model -> Create Index RendererBuffer -> UnsignedShort");
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
									VXM_PROFILE_SCOPE("Model::Model -> Create Index RendererBuffer -> UnsignedInt");
									size_t sizeofValue = sizeof(uint32_t);
									size_t bufferItemsCount = indexSize / sizeofValue;
									const auto *bufferPtr = static_cast<const uint32_t *>(indexBuffer);
									index.insert(index.end(), &bufferPtr[0], &bufferPtr[bufferItemsCount]);
									break;
								}
								default: VXM_CORE_ASSERT(false, "The component type {0} is not supported for an index buffer.", GLTF::Helper::ComponentTypeToString((GLTF::ComponentType) accessor.componentType));
							}
						}
						else {
							VXM_CORE_CHECK(vertexes.size() % 3 == 0, "There is no index buffer and the number of vertex is not divisible by 3 ({})", vertexes.size());
							index.reserve(vertexes.size());
							for (int i = 0; i < (vertexes.size() / 3) * 3; ++i) {
								index.push_back(i);
							}
						}
					}

					MaterialField material;

					// Materials
					{
						VXM_PROFILE_SCOPE("Model::Model -> Create Material");
						if (primitive.material > -1) {
							tinygltf::Material mat = model.materials[primitive.material];

							std::string matName = std::to_string(modelId) + "_" + mat.name;

							if (!vxmModel->m_Materials[primitive.material]) {

								VXM_CORE_INFO("There is {0} material, we want to set the index {1}.", vxmModel->m_Materials.size(), primitive.material);

								MaterialParameters materialParams;

								std::vector<int> materialTextures;
								materialTextures.reserve(5);

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
								materialParams.EmissiveFactor = {mat.emissiveFactor[0], mat.emissiveFactor[1], mat.emissiveFactor[2], 1};
								//							materialParams.SetAlphaMode(mat.alphaMode);
								materialParams.AlphaCutoff = mat.alphaCutoff;
								materialParams.DoubleSided = mat.doubleSided;

								// Textures
								if(materialParams.PbrMetallicRoughness.BaseColorTexture.Index >= 0) materialTextures.push_back(materialParams.PbrMetallicRoughness.BaseColorTexture.Index);
								if(materialParams.NormalTexture.Index >= 0) materialTextures.push_back(materialParams.NormalTexture.Index);
								if(materialParams.PbrMetallicRoughness.MetallicRoughnessTexture.Index >= 0) materialTextures.push_back(materialParams.PbrMetallicRoughness.MetallicRoughnessTexture.Index);
								if(materialParams.OcclusionTexture.Index >= 0) materialTextures.push_back(materialParams.OcclusionTexture.Index);
								if(materialParams.EmissiveTexture.Index >= 0) materialTextures.push_back(materialParams.EmissiveTexture.Index);

								material = assetManager->CreateMemoryAsset<Material>(matName, shader, materialParams);

								for (uint32_t binding : materialTextures)
								{
									material.GetAsset()->SetTexture(textures[binding], binding);
								}

								vxmModel->m_Materials[primitive.material] = material;
							}
							else {
								material = vxmModel->m_Materials[primitive.material];
							}
						}
						else {
							if(vxmModel->m_Materials.size() <= model.materials.size()) {
								MaterialParameters materialParams;
								material = assetManager->CreateMemoryAsset<Material>("Default Material", shader, materialParams);
								vxmModel->m_Materials.push_back(material);
							} else {
								material = vxmModel->m_Materials[model.materials.size()];
							}
						}
					}

					Ref<Mesh> m = assetManager->CreateMemoryAsset<Mesh>(vertexes, index, aabb);
					m->SetMaterial(material);
					meshGroup.AddSubMesh(m);
				}
				vxmModel->m_Meshes.push_back(meshGroup);
			}
		}

		return vxmModel;
	}

	void MeshSerializer::ExportEditorModel(const AssetMetadata& metadata, Ref<Model> model)
	{
		VXM_PROFILE_FUNCTION();
		auto metaFilePath = metadata.FilePath;
		metaFilePath.path += ".meta";

		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("ID", metadata.Handle);
			out << KEYVAL("Type", AssetTypeToString(AssetType::Model));
			MaterialField mat = NullAssetHandle;
			for (auto& matfield : model->m_Materials) {
				if (matfield) {
					mat = matfield;
					break;
				}
			}
			out << KEYVAL("Shader", (mat ? mat.GetAsset()->GetShaderHandle() : NullAssetHandle));
			out << YAML::EndSeq;
		}
		out << YAML::EndMap;

		FileSystem::WriteYamlFile(metaFilePath, out);
	}
} // namespace Voxymore::Core
