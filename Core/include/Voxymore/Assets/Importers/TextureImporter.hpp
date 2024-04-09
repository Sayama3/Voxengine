//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"

namespace Voxymore::Core
{
	class TextureImporter
	{
	public:
		static Ref<Asset> ImportTexture2D(const AssetMetadata& metadata);
	};
}
