//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore::Core
{

	class MaterialImGui
	{
	public:
		static bool OnMaterialImGui(Ref<Asset> asset);
		static bool OnMenuMaterialImGui(AssetMetadata asset);
	};

} // namespace Voxymore::Core

