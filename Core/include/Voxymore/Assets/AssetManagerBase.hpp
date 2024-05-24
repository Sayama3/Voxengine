//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Asset.hpp"
#include "Voxymore/Core/SmartPointers.hpp"


namespace Voxymore::Core
{
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;

	class AssetManagerBase
	{
	public:
		inline virtual ~AssetManagerBase() {}

		[[nodiscard]] virtual bool IsAssetHandleValid(AssetHandle handle) const = 0;
		[[nodiscard]] virtual bool IsAssetLoaded(AssetHandle handle) const = 0;
		[[nodiscard]] virtual AssetType GetAssetType(AssetHandle handle) const = 0;

		virtual Ref<Asset> GetAsset(AssetHandle handle) = 0;
		//TODO: Create Memory Asset ?
	};

} // namespace Voxymore::Core

