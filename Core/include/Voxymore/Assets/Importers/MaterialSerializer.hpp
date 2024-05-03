//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include <any>

#define MaterialExtension ".vxm_material"

namespace Voxymore::Core
{

	class MaterialSerializer
	{
	public:
		static bool IsMaterial(const std::filesystem::path& path);
		static void ExportEditorMaterial(const AssetMetadata& metadata, Ref<Material> material);
		static Ref<Asset> ImportMaterial(const AssetMetadata& metadata);
	};

} // namespace Voxymore::Core

