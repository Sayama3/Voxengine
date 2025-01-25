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

		if (auto fileSource = DynamicCastPtr<FileShaderSource>(shaderSource)) {
			return OnFileShaderSourceImGui(fileSource);
		}

		if (auto memorySource = DynamicCastPtr<MemoryShaderSource>(shaderSource)) {
			return OnMemoryShaderSourceImGui(memorySource);
		}

		VXM_CORE_ASSERT(false, "The Shader Source '{}' has no handled types.", shaderSource->Handle.string());
		return false;
	}

	bool ShaderImGui::OnFileShaderSourceImGui(const Ref<FileShaderSource>& shaderSource) {
		bool changed = false;

		if (shaderSource->HasChanged()) {
			ImGui::TextColored({0.9, 0.1,0.2,1}, "The shader source file has changed.");
		}

		if(ShaderTypeCombo("Shader Type", &shaderSource->Type))
		{
			ShaderSerializer::ExportEditorShaderSource(Project::GetActive()->GetEditorAssetManager()->GetMetadata(shaderSource->Handle), shaderSource);
		}

		if(ImGui::CollapsingHeader("Sources"))
		{
			if (shaderSource->HasChanged()) {
				shaderSource->TryReloadSource();
			}

			if (!shaderSource->SourceRef) {
				shaderSource->SourceRef = CreateRef<std::string>();
			}

			changed |= ImGuiLib::InputTextMultiline("##ShaderSource", shaderSource->SourceRef.get(), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);
		}

		ImGui::BeginDisabled(!FileSystem::Exist(shaderSource->SourcePath));

		if (ImGui::Button("Save")) {
			FileSystem::Write(shaderSource->SourcePath, *shaderSource->SourceRef);
			shaderSource->SourcePathTime = std::filesystem::last_write_time(shaderSource->SourcePath.GetFullPath());
			//TODO?: Auto Reload ?
		}

		ImGui::SameLine();

		if (ImGui::Button("Reload")) {
			shaderSource->ForceReloadSource();
			//TODO?: Auto Reload ?
		}
		ImGui::EndDisabled();

		return changed;
	}

	bool ShaderImGui::OnMemoryShaderSourceImGui(const Ref<MemoryShaderSource>& shaderSource) {

		bool changed = false;

		if(ShaderTypeCombo("Shader Type", &shaderSource->Type))
		{
			ShaderSerializer::ExportEditorShaderSource(Project::GetActive()->GetEditorAssetManager()->GetMetadata(shaderSource->Handle), shaderSource);
		}

		if(ImGui::CollapsingHeader("Sources"))
		{
			changed |= ImGuiLib::InputTextMultiline("##ShaderSource", shaderSource->SourceRef.get(), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);
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

		bool isForward = shader->IsForward();
		if (ImGui::Checkbox("Is Forward", &isForward)) {
			changed = true;
			shader->SetForward(isForward);
		}

		if(ImGui::CollapsingHeader("Sources")) {
			bool shaderSourcesChanged = false;
			std::array<ShaderSourceField, 6> src;

			std::vector<ShaderSourceField> sources = shader->GetSources();

			for (uint8_t i = 1; i < ShaderTypeCount; ++i) {
				auto it = std::find_if(sources.begin(), sources.end(), [i](const ShaderSourceField& src) {return src.IsValid() && src.GetAsset()->GetShaderType() == (ShaderType)i;});
				if(it != sources.end()) src[i] = *it;
				else src[i] = NullAssetHandle;
			}


			uint32_t num = 0;
			for (uint8_t i = 1; i < ShaderTypeCount; ++i)
			{
				auto s = src[i];
				if(ImGuiLib::DrawAssetField(Utils::ShaderTypeToStringBeautify((ShaderType)i).c_str(), &s))
				{
					if(!s || s.GetAsset()->GetShaderType() == (ShaderType)i) {
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

		name = "Set as deferred shader##";
		name += asset.Handle.string();
		if(ImGui::MenuItem(name.c_str())) {
			Project::SetDeferredShader(asset.Handle);
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
			if(!shaderSource || shaderSource.GetAsset()->GetShaderType() == ShaderType::COMPUTE_SHADER)
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

