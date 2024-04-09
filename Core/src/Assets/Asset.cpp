//
// Created by ianpo on 09/04/2024.
//

#include "Asset.hpp"

namespace Voxymore
{
	namespace Core
	{
		std::string_view AssetTypeToString(AssetType type)
		{
			switch (type) {
				case AssetType::None: return "None";
				case AssetType::Scene: return "Scene";
				case AssetType::Texture2D: return "Texture2D";
				case AssetType::Material: return "Material";
				case AssetType::Shader: return "Shader";
			}
			return "";
		}

		AssetType AssetTypeFromString(std::string_view assetType)
		{
			if(assetType == "None") return AssetType::None;
			else if(assetType == "Scene") return AssetType::Scene;
			else if(assetType == "Texture2D") return AssetType::Texture2D;
			else if(assetType == "Material") return AssetType::Material;
			else if(assetType == "Shader") return AssetType::Shader;
			return AssetType::None;
		}
	}// namespace Core
}// namespace Voxymore