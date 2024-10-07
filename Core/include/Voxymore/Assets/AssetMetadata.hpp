//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Assets/Asset.hpp"


namespace Voxymore::Core
{
	struct AssetMetadata {
		inline AssetMetadata() = default;
		inline AssetMetadata(Path p, AssetType t) : FilePath(p), Handle(), Type(t){}
		Path FilePath;
		AssetHandle Handle;
		AssetType Type = AssetType::None;

		inline operator bool() const {
			return Type != AssetType::None && Handle != NullAssetHandle;
		}
		inline bool operator ==(const AssetMetadata& other ) const {
			return Handle == other.Handle && Type == other.Type && FilePath == other.FilePath;
		}
		inline bool operator !=(const AssetMetadata& other) const {
			return !(*this == other);
		}
	};


} // namespace Voxymore::Core

