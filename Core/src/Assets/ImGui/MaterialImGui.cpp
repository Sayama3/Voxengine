//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/ImGui/MaterialImGui.hpp"
#include "Voxymore/Assets/Importers/MaterialSerializer.hpp"

namespace Voxymore::Core
{
	bool MaterialImGui::OnMaterialImGui(Ref<Asset> asset)
	{
		if(asset->GetType() != Material::GetStaticType()) return false;
		Ref<Material> material = CastPtr<Material>(asset);
		bool changed = false;
		changed = material->OnImGui();

		if(ImGui::Button("Save")) {
			auto assetManager = Project::GetActive()->GetEditorAssetManager();
			MaterialSerializer::ExportEditorMaterial(assetManager->GetMetadata(material->Handle), material);
		}
		return changed;
	}

} // namespace Voxymore::Core

