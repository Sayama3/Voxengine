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
#define ComputeShaderExtension ".vxm_compute_shader"

namespace Voxymore::Core
{

	class ShaderSerializer
	{
	public:
		static bool IsShaderSource(const std::filesystem::path& path);
		static Ref<Asset> ImportShaderSource(const AssetMetadata& metadata);
		static void ExportEditorShaderSource(const AssetMetadata& metadata, Ref<ShaderSource> source);

		static bool IsGraphicShader(const std::filesystem::path& path);
		static void ExportEditorGraphicShader(const AssetMetadata& metadata, Ref<Shader> shader);
		static Ref<Asset> ImportGraphicShader(const AssetMetadata& metadata);

		static bool IsComputeShader(const std::filesystem::path& path);
		static void ExportEditorComputeShader(const AssetMetadata& metadata, Ref<ComputeShader> shader);
		static Ref<Asset> ImportComputeShader(const AssetMetadata& metadata);
	};

} // namespace Voxymore::Core

