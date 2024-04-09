//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Assets/AssetImporter.hpp"

namespace Voxymore::Core
{
		Ref<Asset> AssetImporter::ImportAsset(const AssetMetadata &metadata)
		{
			VXM_PROFILE_FUNCTION();
			auto it = AssetLoaders.find(metadata.Type);
			if(it == AssetLoaders.end()) {
				VXM_CORE_ERROR("No importer for type {0}.", AssetTypeToString(metadata.Type));
				return nullptr;
			}
			Ref<Asset> asset = it->second(metadata);
			return asset;
		}
}// namespace Voxymore::Core