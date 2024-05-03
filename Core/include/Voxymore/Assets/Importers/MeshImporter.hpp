//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Mesh.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include <any>


#define VXM_EXTENSION_GLTF ".gltf"
#define VXM_EXTENSION_GLB ".glb"

namespace Voxymore::Core
{
	class MeshSerializer
	{
		//TODO: Import using assimp ?
		static const inline std::vector<std::string> ModelExtensions = {VXM_EXTENSION_GLB, VXM_EXTENSION_GLTF};
		static inline const char* const MeshExtension = ".vxm_mesh";
	public:
		static bool IsMesh(const std::filesystem::path& path);
		static void ExportEditorMesh(const AssetMetadata& metadata, Ref<Mesh> mesh);
		static Ref<Asset> ImportMesh(const AssetMetadata& metadata);

		static bool IsModel(const std::filesystem::path& path);
		static void ExportEditorModel(const AssetMetadata& metadata, Ref<Model> model);
		static Ref<Asset> ImportModel(const AssetMetadata& metadata);
	};
} // namespace Voxymore::Core

