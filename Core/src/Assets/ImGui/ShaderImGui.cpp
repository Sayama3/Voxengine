//
// Created by ianpo on 12/04/2024.
//

#include "Voxymore/Assets/ImGui/ShaderImGui.hpp"
#include "Voxymore/Assets/Importers/ShaderSerializer.hpp"

namespace Voxymore::Core
{

	bool ShaderImGui::ShaderTypeCombo(const char *label, ShaderType* shaderType)
	{
		bool changed = false;

		std::string shaderTypeName = Utils::ShaderTypeToStringBeautify(*shaderType);

		if(ImGui::BeginCombo("Shader Type", shaderTypeName.c_str()))
		{
			for (int i = 0; i <= ShaderTypeCount; ++i) {
				auto type = (ShaderType)i;
				const bool is_selected = type == (*shaderType);
				if(ImGui::Selectable(Utils::ShaderTypeToStringBeautify(type).c_str(), is_selected))
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

	bool ShaderImGui::OnGraphicShaderImGui(Ref<Asset> asset)
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
			std::array<ShaderSourceField, 6> src;

			std::vector<ShaderSourceField> sources = shader->GetSources();

			for (uint8_t i = 1; i < ShaderTypeCount; ++i) {
				auto it = std::find_if(sources.begin(), sources.end(), [i](const ShaderSourceField& src) {return src.IsValid() && src.GetAsset()->Type == (ShaderType)i;});
				if(it != sources.end()) src[i] = *it;
				else src[i] = NullAssetHandle;
			}


			uint32_t num = 0;
			for (uint8_t i = 1; i < ShaderTypeCount; ++i)
			{
				auto s = src[i];
				if(ImGuiLib::DrawAssetField(Utils::ShaderTypeToStringBeautify((ShaderType)i).c_str(), &s))
				{
					if(!s || s.GetAsset()->Type == (ShaderType)i) {
						src[i] = s;
						shaderSourcesChanged = true;
					}
				}
			}

			if(shaderSourcesChanged) {
				sources.clear();
				sources.reserve(ShaderTypeCount - 1);
				sources.insert(sources.end(), src.begin() + 1, src.end());

				changed = true;
				shader->SetSources(sources);
			}
		}

		if(ImGui::Button("Reload")) {
			shader->Reload();
		}

		if(ImGui::Button("Save")) {
			auto assetManager = Project::GetActive()->GetEditorAssetManager();
			ShaderSerializer::ExportEditorGraphicShader(assetManager->GetMetadata(shader->Handle), shader);
		}

		return changed;
	}

	bool ShaderImGui::OnMenuShaderImGui(AssetMetadata asset)
	{
		if(asset.Type != AssetType::Shader) return false;

		std::string name = "Set as default shader##";
		name += asset.Handle.string();
		if(ImGui::MenuItem(name.c_str())) {
			Project::SetDefaultShader(asset.Handle);
			Project::SaveActive();
			return true;
		}

		name = "Set as gizmo shader##";
		name += asset.Handle.string();
		if(ImGui::MenuItem(name.c_str())) {
			Project::SetGizmoShader(asset.Handle);
			Project::SaveActive();
			return true;
		}
		return false;
	}


	bool ShaderImGui::OnComputeShaderImGui(Ref<Asset> asset)
	{
		if(asset->GetType() != ComputeShader::GetStaticType()) return false;
		Ref<ComputeShader> shader = CastPtr<ComputeShader>(asset);
		bool changed = false;
		std::string name = shader->GetName();

		if(ImGuiLib::InputText("Name", &name)) {
			changed = true;
			shader->SetName(name);
		}

		AssetField<ShaderSource> shaderSource = shader->GetSource();
		if(ImGuiLib::DrawAssetField(Utils::ShaderTypeToStringBeautify(ShaderType::COMPUTE_SHADER).c_str(), &shaderSource))
		{
			if(!shaderSource || shaderSource.GetAsset()->Type == ShaderType::COMPUTE_SHADER)
			{
				changed = true;
				shader->SetSource(shaderSource);
			}
		}

		if(ImGui::Button("Reload")) {
			shader->Reload();
		}

		if(ImGui::Button("Save")) {
			auto assetManager = Project::GetActive()->GetEditorAssetManager();
			ShaderSerializer::ExportEditorComputeShader(assetManager->GetMetadata(shader->Handle), shader);
		}

		return changed;
	}
} // namespace Voxymore::Core

