//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/ImGui/ShaderImGui.hpp"
#include "Voxymore/Assets/Importers/ShaderSerializer.hpp"

namespace Voxymore::Core
{
	bool ShaderImGui::OnShaderSourceImGui(Ref<Asset> asset)
	{
		if(asset->GetType() != ShaderSource::GetStaticType()) return false;
		Ref<ShaderSource> shaderSource = CastPtr<ShaderSource>(asset);
		bool changed = false;

		std::string shaderTypeName = Utils::ShaderTypeToString(shaderSource->Type);

		if(ImGui::BeginCombo("Shader Type", shaderTypeName.c_str()))
		{
			for (int i = 0; i <= ShaderTypeCount; ++i) {
				const bool is_selected = (i == (int)shaderSource->Type);
				ShaderType type = (ShaderType)i;
				if(ImGui::Selectable(Utils::ShaderTypeToString(type).c_str(), is_selected))
				{
					changed |= shaderSource->Type != type;
					shaderSource->Type = type;
					ShaderSerializer::ExportEditorShaderSource(Project::GetActive()->GetEditorAssetManager()->GetMetadata(shaderSource->Handle), shaderSource);
				}

				if(is_selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if(ImGui::CollapsingHeader("Sources"))
		{
			if(shaderSource->Source.capacity() < shaderSource->Source.size() * 1.25) shaderSource->Source.reserve(shaderSource->Source.size() * 2);
			ImGui::InputTextMultiline("##ShaderSource", shaderSource->Source.data(), shaderSource->Source.capacity(), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);
		}

		if (ImGui::Button("Save")) {
			auto assetManager = Project::GetActive()->GetEditorAssetManager();
			auto path = assetManager->GetFilePath(shaderSource->Handle);
			FileSystem::Write(path, shaderSource->Source.c_str(), shaderSource->Source.size());
			//TODO: Auto Reload ?
		}

		return changed;
	}

	bool ShaderImGui::OnShaderImGui(Ref<Asset> asset)
	{
		if(asset->GetType() != Shader::GetStaticType()) return false;
		Ref<Shader> shader = CastPtr<Shader>(asset);
		bool changed = false;

		if(ImGui::CollapsingHeader("Sources")) {
			bool shaderSourcesChanged = false;
			std::vector<ShaderSourceField> sources = shader->GetSources();
			sources.resize(std::max(sources.size(), (size_t)ShaderTypeCount));
			uint32_t num = 0;
			for (auto &source: sources) {
				shaderSourcesChanged |= ImGuiLib::DrawAssetField(std::string("##"+std::to_string(num++)).c_str(), &source);
			}
			if(shaderSourcesChanged) {
				changed = true;
				shader->SetSources(sources);
			}
		}

		if(ImGui::Button("Reload")) {
			shader->Reload();
		}

		if(ImGui::Button("Save")) {
			auto assetManager = Project::GetActive()->GetEditorAssetManager();
			ShaderSerializer::ExportEditorShader(assetManager->GetMetadata(shader->Handle), shader);
		}

		return changed;
	}
} // namespace Voxymore::Core

