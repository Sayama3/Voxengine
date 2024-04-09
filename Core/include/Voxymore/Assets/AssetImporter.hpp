//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Asset.hpp"
#include "AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Importers/TextureImporter.hpp"
#include <map>

namespace Voxymore::Core
{
	using AssetImportFunction = std::function<Ref<Asset>(const AssetMetadata&)>;
	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(const AssetMetadata& metadata);
		static inline std::map<AssetType, AssetImportFunction> AssetLoaders = {
				{AssetType::Texture2D, TextureImporter::ImportTexture2D},
		};
	};

} // namespace Voxymore::Core

