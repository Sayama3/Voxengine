//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Scene/Scene.hpp"
#include <any>

#define SceneExtension ".vxm_scn"

namespace Voxymore::Core
{

	class SceneImporter
	{
	public:
		static bool IsScene(const std::filesystem::path& path);
		static Ref<Asset> ImportScene(const AssetMetadata& metadata);
		static void ExportEditorScene(const AssetMetadata& metadata, Ref<Scene> scene);
	};

} // namespace Voxymore::Core

