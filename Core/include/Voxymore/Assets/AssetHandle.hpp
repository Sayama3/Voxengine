//
// Created by Sayama on 05/12/2024.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"

namespace Voxymore::Core {

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

    using AssetHandle = UUID;

    std::string AssetTypeToString(AssetType type);
    AssetType AssetTypeFromString(const std::string& assetType);
    bool TryAssetTypeFromString(const std::string& assetType, AssetType& type);

    std::string AssetTypeToPayloadID(AssetType assetType);

    [[maybe_unused]] static inline constexpr AssetHandle NullAssetHandle{0};
    //	using OptionalAssetHandle = Optional<AssetHandle, uint64_t, 0>;
}
