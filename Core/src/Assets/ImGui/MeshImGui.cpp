//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/ImGui/MeshImGui.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core
{
	bool MeshImGui::OnMeshImGui(Ref<Asset> asset)
	{
		return false;
	}

	bool MeshImGui::OnModelImGui(Ref<Asset> asset)
	{
		if(asset->GetType() != Model::GetStaticType()) return false;
		Ref<Model> model = CastPtr<Model>(asset);
		bool changed = false;
		ImGui::BeginDisabled(true);
		changed |= ImGuiLib::InputUUID("Model ID", &model->Handle);
		ImGui::EndDisabled();

		static ShaderField shaderField;

		ImGuiLib::DrawAssetField("Shader", &shaderField);

		if(ImGui::Button("Set Shader On All Materials")) {
			for(auto& mat : model->m_Materials)
			{
				if(mat) {
					mat.GetAsset()->ChangeShader(shaderField.GetHandle());
				}
			}
			changed = true;
		}

		return changed;
	}
} // namespace Voxymore::Core

