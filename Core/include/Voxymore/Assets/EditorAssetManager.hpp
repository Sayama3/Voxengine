//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "AssetManagerBase.hpp"
#include "AssetMetadata.hpp"
#include "AssetImporter.hpp"
#include <map>

namespace Voxymore::Core
{
	// ordered map is cool for debugging
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class EditorAssetManager : public AssetManagerBase
	{
	public:
		virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;
		virtual Ref<Asset> GetAsset(AssetHandle handle) override;
	public:
		[[nodiscard]] const AssetMetadata& GetMetadata(AssetHandle handle) const;
	private:
		AssetMap m_LoadedAssets;
		AssetRegistry m_AssetRegistry;

		//TODO: Memory-only assets;
	};

} // namespace Voxymore::Core

