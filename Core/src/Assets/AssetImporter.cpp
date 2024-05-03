//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Assets/AssetImporter.hpp"
#include "Voxymore/Assets/Importers/TextureImporter.hpp"
#include "Voxymore/Assets/Importers/MaterialSerializer.hpp"
#include "Voxymore/Assets/Importers/MeshImporter.hpp"
#include "Voxymore/Assets/Importers/ShaderSerializer.hpp"
#include "Voxymore/Assets/Importers/SceneImporter.hpp"

namespace Voxymore::Core
{
	std::unordered_map<AssetType, AssetImportFunction> AssetImporter::AssetLoaders = {
			{AssetType::Texture2D, TextureImporter::ImportTexture2D},
			//			{AssetType::CubeMap, TextureImGui::ImportCubeMap},
			{AssetType::Shader, ShaderSerializer::ImportShader},
			{AssetType::ShaderSource, ShaderSerializer::ImportShaderSource},
//			{AssetType::Mesh, MeshSerializer::ImportMesh},
			{AssetType::Scene, SceneImporter::ImportScene},
			{AssetType::Material, MaterialSerializer::ImportMaterial},
			{AssetType::Model, MeshSerializer::ImportModel},
	};

	std::unordered_map<AssetType, AssetDetectorFunction> AssetImporter::AssetDetectors = {
			{AssetType::Texture2D, TextureImporter::IsTexture},
//			{AssetType::CubeMap, TextureImporter::IsCubeMap},
			{AssetType::Shader, ShaderSerializer::IsShader},
			{AssetType::ShaderSource, ShaderSerializer::IsShaderSource},
			//			{AssetType::Mesh, MeshSerializer::IsMesh},
			{AssetType::Scene, SceneImporter::IsScene},
			{AssetType::Material, MaterialSerializer::IsMaterial},
			{AssetType::Model, MeshSerializer::IsModel},
	};

	Ref<Asset> AssetImporter::ImportAsset(const AssetMetadata &metadata)
	{
		VXM_PROFILE_FUNCTION();
		auto it = AssetLoaders.find(metadata.Type);
		if(it == AssetLoaders.end()) {
			VXM_CORE_ERROR("No importer for type {0}.", AssetTypeToString(metadata.Type));
			return nullptr;
		}
		Ref<Asset> asset = it->second(metadata);
		if(asset) asset->Handle = metadata.Handle;
		return asset;
	}

	AssetType AssetImporter::GetAssetType(const Path &path)
	{
		VXM_PROFILE_FUNCTION();
		auto it = std::find_if(AssetDetectors.begin(), AssetDetectors.end(), [&path](const std::pair<AssetType, AssetDetectorFunction>& pair){return pair.second(path);});
		if(it == AssetDetectors.end()) {
//			VXM_CORE_ERROR("Unknown type for asset '{}'.", path.string());
			return AssetType::None;
		}
		else {
			return it->first;
		}
	}
}// namespace Voxymore::Core