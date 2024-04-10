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

		inline operator bool() {return Type == AssetType::None;}
	};


} // namespace Voxymore::Core

