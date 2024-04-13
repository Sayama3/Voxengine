//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Asset.hpp"
#include "AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include <unordered_map>
#include <any>

namespace Voxymore::Core
{
	using AssetImportFunction = std::function<Ref<Asset>(const AssetMetadata&)>;
	using AssetDetectorFunction = std::function<bool(const Path&)>;
	class AssetImporter
	{
	public:
		static AssetType GetAssetType(const Path& path);
		static Ref<Asset> ImportAsset(const AssetMetadata& metadata);
	private:
		static std::unordered_map<AssetType, AssetImportFunction> AssetLoaders;
		static std::unordered_map<AssetType, AssetDetectorFunction> AssetDetectors;
	};

} // namespace Voxymore::Core

