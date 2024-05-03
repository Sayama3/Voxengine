//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Assets/Asset.hpp"


namespace Voxymore::Core
{
	std::string AssetTypeToString(AssetType type)
	{
		switch (type) {
			case AssetType::None: return "AssetType::None";
			case AssetType::Scene: return "AssetType::Scene";
			case AssetType::Texture2D: return "AssetType::Texture2D";
			case AssetType::CubeMap: return "AssetType::CubeMap";
			case AssetType::Material: return "AssetType::Material";
			case AssetType::Shader: return "AssetType::Shader";
			case AssetType::ShaderSource: return "AssetType::ShaderSource";
			case AssetType::Model: return "AssetType::Model";
			case AssetType::Mesh: return "AssetType::Mesh";
		}
		VXM_CORE_ASSERT(false, "The type '{}' is unknown", (uint16_t)type);
		return "AssetType::<Unknown>";
	}

	bool TryAssetTypeFromString(const std::string& assetType, AssetType& type)
	{
		if(assetType == "AssetType::None") {
			type = AssetType::None;
			return true;
		}
		else if(assetType == "AssetType::Scene") {
			type = AssetType::Scene;
			return true;
		}
		else if(assetType == "AssetType::Texture2D") {
			type = AssetType::Texture2D;
			return true;
		}
		else if(assetType == "AssetType::CubeMap") {
			type = AssetType::CubeMap;
			return true;
		}
		else if(assetType == "AssetType::Material") {
			type = AssetType::Material;
			return true;
		}
		else if(assetType == "AssetType::Shader") {
			type = AssetType::Shader;
			return true;
		}
		else if(assetType == "AssetType::ShaderSource") {
			type = AssetType::ShaderSource;
			return true;
		}
		else if(assetType == "AssetType::Model") {
			type = AssetType::Model;
			return true;
		}
		else if(assetType == "AssetType::Mesh") {
			type = AssetType::Mesh;
			return true;
		}
		return false;
	}

	AssetType AssetTypeFromString(const std::string& assetType)
	{
		AssetType t = AssetType::None;
		if(!TryAssetTypeFromString(assetType, t)) {
			VXM_CORE_ERROR("The type '{}' is unknown", assetType);
			return AssetType::None;
		}
		return t;
	}

	std::string AssetTypeToPayloadID(AssetType assetType)
	{
		return std::string("VXM_ASSET_") + std::to_string(static_cast<uint16_t>(assetType));
	}

} // namespace Voxymore::Core
