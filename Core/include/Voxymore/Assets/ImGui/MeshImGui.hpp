//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetMetadata.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Mesh.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

namespace Voxymore::Core
{
	class MeshImGui
	{
	public:
		static bool OnMeshImGui(Ref<Asset> asset);

		static bool OnModelImGui(Ref<Asset> asset);
	};
} // namespace Voxymore::Core

