//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore::Core
{

	class ShaderImGui
	{
	public:
		static bool ShaderTypeCombo(const char* label, ShaderType* shaderType);

		static bool OnShaderSourceImGui(Ref<Asset> asset);

		static bool OnGraphicShaderImGui(Ref<Asset> asset);
		static bool OnComputeShaderImGui(Ref<Asset> asset);

		static bool OnMenuShaderImGui(AssetMetadata asset);
	};

} // namespace Voxymore::Core

