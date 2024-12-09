//
// Created by Sayama on 05/12/2024.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"

namespace Voxymore::Core {
    using AssetHandle = UUID;
    [[maybe_unused]] static inline constexpr AssetHandle NullAssetHandle{0};
    //	using OptionalAssetHandle = Optional<AssetHandle, uint64_t, 0>;
}
