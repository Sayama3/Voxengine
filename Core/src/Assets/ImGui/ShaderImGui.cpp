//
// Created by ianpo on 12/04/2024.
//

#pragma once

#include "Voxymore/Assets/ImGui/ShaderImGui.hpp"
#include "Voxymore/Assets/Importers/ShaderSerializer.hpp"

namespace Voxymore::Core
{

	bool ShaderImGui::ShaderTypeCombo(const char *label, ShaderType* shaderType)
	{
		bool changed = false;

		std::string shaderTypeName = Utils::ShaderTypeToString(*shaderType);

		if(ImGui::BeginCombo("Shader Type", shaderTypeName.c_str()))
		{
			for (int i = 0; i <= ShaderTypeCount; ++i) {
				auto type = (ShaderType)i;
				const bool is_selected = type == (*shaderType);
				if(ImGui::Selectable(Utils::ShaderTypeToString(type).c_str(), is_selected))
				{
					changed |= (*shaderType) != type;
					(*shaderType) = type;
					//ShaderSerializer::ExportEditorShaderSource(Project::GetActive()->GetEditorAssetManager()->GetMetadata(shaderSource->Handle), shaderSource);
				}

				if(is_selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		return changed;
	}
	bool ShaderImGui::OnShaderSourceImGui(Ref<Asset> asset)
	{
		if(asset->GetType() != ShaderSource::GetStaticType()) return false;
		Ref<ShaderSource> shaderSource = CastPtr<ShaderSource>(asset);
		bool changed = false;

		if(ShaderTypeCombo("Shader Type", &shaderSource->Type))
		{
			ShaderSerializer::ExportEditorShaderSource(Project::GetActive()->GetEditorAssetManager()->GetMetadata(shaderSource->Handle), shaderSource);
		}

		if(ImGui::CollapsingHeader("Sources"))
		{
			changed |= ImGuiLib::InputTextMultiline("##ShaderSource", &(shaderSource->Source), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);
		}

		if (ImGui::Button("Save")) {
			auto assetManager = Project::GetActive()->GetEditorAssetManager();
			auto path = assetManager->GetFilePath(shaderSource->Handle);
			FileSystem::Write(path, shaderSource->Source.c_str(), shaderSource->Source.size());
			//TODO: Auto Reload ?
		}

		ImGui::SameLine();

		if (ImGui::Button("Reload")) {
			auto assetManager = Project::GetActive()->GetEditorAssetManager();
			auto path = assetManager->GetFilePath(shaderSource->Handle);
			shaderSource->Source = FileSystem::ReadFileAsString(path);
			//TODO: Auto Reload ?
		}

		return changed;
	}

	bool ShaderImGui::OnShaderImGui(Ref<Asset> asset)
	{
		if(asset->GetType() != Shader::GetStaticType()) return false;
		Ref<Shader> shader = CastPtr<Shader>(asset);
		bool changed = false;
		std::string name = shader->GetName();

		if(ImGuiLib::InputText("Name", &name)) {
			shader->SetName(name);
		}

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

