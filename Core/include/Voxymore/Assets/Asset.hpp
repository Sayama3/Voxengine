//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Voxymore/Assets/AssetHandle.hpp"

#define VXM_IMPLEMENT_ASSET(assetType) inline static AssetType GetStaticType() {return assetType;} \
inline virtual AssetType GetType() const override {return GetStaticType();}


namespace Voxymore::Core
{

	class Asset
	{
	public:
		virtual ~Asset() = default;

		AssetHandle Handle; // Generate handle
		virtual AssetType GetType() const = 0;
	};

} // namespace Voxymore::Core

