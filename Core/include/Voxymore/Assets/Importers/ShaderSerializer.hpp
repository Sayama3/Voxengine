//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include <any>


#define ShaderExtension ".vxm_shader"

namespace Voxymore::Core
{

	class ShaderSerializer
	{
	public:
		static bool IsShaderSource(const std::filesystem::path& path);
		static Ref<Asset> ImportShaderSource(const AssetMetadata& metadata);
		static void ExportEditorShaderSource(const AssetMetadata& metadata, Ref<ShaderSource> source);

		static bool IsShader(const std::filesystem::path& path);
		static void ExportEditorShader(const AssetMetadata& metadata, Ref<Shader> shader);
		static Ref<Asset> ImportShader(const AssetMetadata& metadata);
	};

} // namespace Voxymore::Core

