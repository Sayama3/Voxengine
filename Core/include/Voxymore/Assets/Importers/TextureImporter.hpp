//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include <any>

namespace Voxymore::Core
{
	class TextureImporter
	{
	public:
		static bool IsTexture(const std::filesystem::path& path);
		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);
		static void ExportEditorTexture2D(const AssetMetadata& metadata, Ref<Texture2D> texture);
		static Ref<Asset> ImportTexture2D(const AssetMetadata& metadata);

	};
}
