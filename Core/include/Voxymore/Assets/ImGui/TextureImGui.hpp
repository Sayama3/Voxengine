//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore::Core
{
	class TextureImGui
	{
	public:
		static bool OnTexture2DImGui(Ref<Asset> asset);
		static bool OnCubemapImGui(Ref<Asset> asset);
	};
}
