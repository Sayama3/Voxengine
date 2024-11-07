//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include <string>
#include "Voxymore/Core/Optional.hpp"
#include "Voxymore/Core/UUID.hpp"

#define VXM_IMPLEMENT_ASSET(assetType) inline static AssetType GetStaticType() {return assetType;} \
inline virtual AssetType GetType() const override {return GetStaticType();}


namespace Voxymore::Core
{
	using AssetHandle = UUID;
	[[maybe_unused]] static inline constexpr AssetHandle NullAssetHandle{0};
//	using OptionalAssetHandle = Optional<AssetHandle, uint64_t, 0>;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D,
		CubeMap,
		ShaderSource,
		Shader,
		ComputeShader,
		Material,
		Model,
		Mesh,
	};

	std::string AssetTypeToString(AssetType type);
	AssetType AssetTypeFromString(const std::string& assetType);
	bool TryAssetTypeFromString(const std::string& assetType, AssetType& type);

	std::string AssetTypeToPayloadID(AssetType assetType);

	class Asset
	{
	public:
		AssetHandle Handle; // Generate handle
		virtual AssetType GetType() const = 0;
	};

} // namespace Voxymore::Core

