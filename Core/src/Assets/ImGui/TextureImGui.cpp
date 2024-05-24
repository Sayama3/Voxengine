//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "Voxymore/Assets/ImGui/TextureImGui.hpp"

namespace Voxymore::Core
{
	bool TextureImGui::OnTexture2DImGui(Ref<Asset> asset)
	{
		return false;
	}

	bool TextureImGui::OnCubemapImGui(Ref<Asset> asset)
	{
		return false;
	}
}
