//
// Created by ianpo on 01/08/2023.
//

#include <utility>

#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/ImGui/ImGuiLib.hpp"

#define SerializeSerializable(Emitter, Param) Emitter << KEYVAL(#Param, YAML::BeginMap); \
Param.Serialize(Emitter); \
Emitter << YAML::EndMap

#define SerializeSerializableIf(Condition, Emitter, Param) if(Condition) {SerializeSerializable(Emitter, Param);}

#define DeserializeSerializableIf(Condition, node, Param) auto VXM_COMBINE(Param, Node) = node[#Param]; \
Condition = VXM_COMBINE(Param, Node).IsDefined(); \
if(Condition) Param.Deserialize(VXM_COMBINE(Param, Node))

#define DeserializeSerializable(node, Param) auto VXM_COMBINE(Param, Node) = node[#Param]; \
Param.Deserialize(VXM_COMBINE(Param, Node))

namespace Voxymore::Core {
	// ===== Material =====

	void Material::Serialize(YAML::Emitter &out) const
	{
        VXM_PROFILE_FUNCTION();
		out << KEYVAL("MaterialName", m_MaterialName);
		out << KEYVAL("Shader", m_Shader);
		out << KEYVAL("Textures", YAML::BeginSeq);{
			for (int i = 0; i < m_Textures.size(); ++i) {
				out <<  m_Textures[i];
			}
			out << YAML::EndSeq;
		}
		out << KEYVAL("Parameters", YAML::BeginMap);
		MaterialParameters_Serialize(&m_Parameters, out);
		out << YAML::EndMap;
	}

	void Material::Deserialize(YAML::Node &node)
	{
        VXM_PROFILE_FUNCTION();
		m_MaterialName = node["MaterialName"].as<std::string>();
		m_Shader = node["Shader"].as<ShaderField>();
		auto texturesNode = node["Textures"];
		if(texturesNode && texturesNode.IsSequence()) {
			for (int i = 0; i < m_Textures.size(); ++i) {
				m_Textures[i] = texturesNode[i].as<Texture2DField>();
			}
		}
		auto ParameterNode = node["Parameters"];
		MaterialParameters_Deserialize(&m_Parameters, ParameterNode);
	}

	Material::Material()
	{
	}

	Material::Material(const std::string& name) : m_MaterialName(name)
	{
	}

	Material::Material(const ShaderField &shader) : m_Shader(shader)
	{
	}

	Material::Material(const ShaderField& shader, const MaterialParameters& parameters) : m_Shader(shader), m_Parameters(parameters)
	{
	}

	Material::Material(const std::string& name, const ShaderField& shader, const MaterialParameters& parameters) : m_MaterialName(name), m_Shader(shader), m_Parameters(parameters)
	{
	}

	Material::Material(const MaterialParameters& parameters) : m_Parameters(parameters)
	{
	}
	Material::Material(const std::string& name, const MaterialParameters& parameters) : m_MaterialName(name), m_Parameters(parameters)
	{
	}

	Material::~Material()
	{
	}

	void Material::Bind(bool bindShader) const
	{
        VXM_PROFILE_FUNCTION();

		if (!m_Shader) {
			VXM_CORE_ERROR("The shader is not valid. Aborting.");
			return;
		}

		if(bindShader) {
			m_Shader.GetAsset()->Bind();
		}

		for (int i = 0; i < m_Textures.size(); ++i) {
			if(m_Textures[i])
			{
				m_Textures[i].GetAsset()->Bind(i);
			}
		}
	}

	void Material::Unbind() const
	{
		VXM_PROFILE_FUNCTION();
		if(!m_Shader) return;
//		VXM_CORE_ASSERT(m_Shader, "The shader is not loaded.");
		m_Shader.GetAsset()->Unbind();
		for (int i = 0; i < m_Textures.size(); ++i)
		{
			if(m_Textures[i])
			{
				Texture2D::Unbind(i);
			}
		}
	}

	void Material::SetTexture(Texture2DField texture, int binding)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(binding >= 0 && binding < m_Textures.max_size(), "The texture binding {0} is not valid.", binding);
		if(binding >= 0 && binding < m_Textures.max_size()) return;
		m_Textures[binding] = std::move(texture);
	}

	void Material::UnsetTexture(int binding)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(binding >= 0 && binding < m_Textures.max_size(), "The texture binding {0} is not valid.", binding);
		if(binding >= 0 && binding < m_Textures.max_size()) return;
		m_Textures[binding].Reset();
	}

	void Material::ChangeShader(AssetHandle shaderHandle)
	{
        VXM_PROFILE_FUNCTION();
		ResetShader();
		m_Shader.SetHandle(shaderHandle);
	}

	void Material::ChangeShader(const Ref<Shader>& shader)
	{
        VXM_PROFILE_FUNCTION();
		ResetShader();
		m_Shader = shader;
	}

	const std::string &Material::GetMaterialName() const
	{
        VXM_PROFILE_FUNCTION();
		return m_MaterialName;
	}

	const MaterialParameters& Material::GetMaterialsParameters() const
	{
        VXM_PROFILE_FUNCTION();
		return m_Parameters;
	}

	MaterialParameters& Material::GetMaterialsParameters()
	{
		VXM_PROFILE_FUNCTION();
		return m_Parameters;
	}

	bool Material::OnImGui()
	{
		VXM_PROFILE_FUNCTION();
		bool changed = false;
		// Create a char buffer
		// Initial string size
		const int inputSize = 256;

		changed |= ImGuiLib::DrawAssetField("Shader", &m_Shader);

		std::vector<char> materialName(inputSize, 0);
		std::memcpy(materialName.data(), m_MaterialName.c_str(), m_MaterialName.size());
		if (ImGui::InputText("Material Name", materialName.data(), inputSize))
		{
			changed = true;
			m_MaterialName = std::string(materialName.data());
		}

		if(ImGui::CollapsingHeader("Textures"))
		{
			for (int i = 0; i < m_Textures.size(); ++i)
			{
				std::string name = "Texture - " + std::to_string(i);
				auto& texture = m_Textures[i];
				changed |= ImGuiLib::DrawAssetField(name.c_str(), &(m_Textures[i]));
			}
		}

		if(ImGui::CollapsingHeader("Parameters"))
		{
			changed |= MaterialParameters_ImGui(&m_Parameters);
		}

		return changed;
	}

	void Material::SetMaterialName(const std::string &name)
	{
        VXM_PROFILE_FUNCTION();
		m_MaterialName = name;
	}

	void Material::ResetShader()
	{
        VXM_PROFILE_FUNCTION();
		m_Shader.Reset();
	}

	// ===== Helper =====
	void MaterialParameters_Deserialize(MaterialParameters* material, YAML::Node& node)
	{
		VXM_PROFILE_FUNCTION();
		if(material == nullptr) return;
		MaterialParameters& mat = *material;
		if(node["AlphaMode"].IsDefined()) {
			mat.AlphaMode = node["AlphaMode"].as<int>();
		} else {
			mat.AlphaMode = AlphaMode::Opaque;
		}
		if(node["AlphaCutoff"].IsDefined()) {
			mat.AlphaCutoff = node["AlphaCutoff"].as<float>();
		} else {
			mat.AlphaCutoff = 0.5f;
		}
		if(node["DoubleSided"].IsDefined()) {
			mat.DoubleSided = node["DoubleSided"].as<int>();
		} else {
			mat.DoubleSided = false;
		}
		if(node["GammaCorrection"].IsDefined()) {
			mat.GammaCorrection = node["GammaCorrection"].as<int>();
		} else {
			mat.GammaCorrection = false;
		}

		{
			auto& metal = mat.PbrMetallicRoughness;
			if(node["BaseColor"].IsDefined()) {
				metal.BaseColorFactor = node["BaseColor"].as<glm::vec4>();
			} else {
				metal.BaseColorFactor = {1,1,1,1};
			}
			if(node["BaseColorIndex"].IsDefined()) {
				metal.BaseColorTexture.Index = node["BaseColorIndex"].as<int>();
			} else {
				metal.BaseColorTexture.Index = -1;
			}
			if(node["BaseColorTexCoord"].IsDefined()) {
				metal.BaseColorTexture.TexCoord = node["BaseColorTexCoord"].as<int>();
			} else {
				metal.BaseColorTexture.TexCoord = 0;
			}

			if(node["MetallicFactor"].IsDefined()) {
				metal.MetallicFactor = node["MetallicFactor"].as<float>();
			} else {
				metal.MetallicFactor = 1;
			}
			if(node["RoughnessFactor"].IsDefined()) {
				metal.RoughnessFactor = node["RoughnessFactor"].as<float>();
			} else {
				metal.RoughnessFactor = 1;
			}
			if(node["MetallicRoughnessIndex"].IsDefined()) {
				metal.MetallicRoughnessTexture.Index = node["MetallicRoughnessIndex"].as<int>();
			} else {
				metal.MetallicRoughnessTexture.Index = -1;
			}
			if(node["MetallicRoughnessTexCoord"].IsDefined()) {
				metal.MetallicRoughnessTexture.TexCoord = node["MetallicRoughnessTexCoord"].as<int>();
			} else {
				metal.MetallicRoughnessTexture.TexCoord = 0;
			}
		}

		{
			auto& normal = mat.NormalTexture;
			if(node["NormalMapIndex"].IsDefined()) {
				normal.Index = node["NormalMapIndex"].as<int>();
			} else {
				normal.Index = -1;
			}
			if(node["NormalMapTexCoord"].IsDefined()) {
				normal.TexCoord = node["NormalMapTexCoord"].as<int>();
			} else {
				normal.TexCoord = 0;
			}
			if(node["NormalMapScale"].IsDefined()) {
				normal.Scale = node["NormalMapScale"].as<int>();
			} else {
				normal.Scale = 1;
			}
		}

		{
			auto& occlusion = mat.OcclusionTexture;
			if(node["OcclusionTextureIndex"].IsDefined()) {
				occlusion.Index = node["OcclusionTextureIndex"].as<int>();
			} else {
				occlusion.Index = -1;
			}
			if(node["OcclusionTextureTexCoord"].IsDefined()) {
				occlusion.TexCoord = node["OcclusionTextureTexCoord"].as<int>();
			} else {
				occlusion.TexCoord = 0;
			}
			if(node["OcclusionTextureStrength"].IsDefined()) {
				occlusion.Strength = node["OcclusionTextureStrength"].as<int>();
			} else {
				occlusion.Strength = 1;
			}
		}

		{
			auto& emissive = mat.EmissiveTexture;
			if(node["EmissiveFactor"].IsDefined()) {
				mat.EmissiveFactor = node["EmissiveFactor"].as<glm::vec4>();
			} else {
				mat.EmissiveFactor = {0,0,0,1};
			}
			if(node["EmissiveTextureIndex"].IsDefined()) {
				emissive.Index = node["EmissiveTextureIndex"].as<int>();
			} else {
				emissive.Index = 1;
			}
			if(node["EmissiveTextureTexCoord"].IsDefined()) {
				emissive.TexCoord = node["EmissiveTextureTexCoord"].as<int>();
			} else {
				emissive.TexCoord = 1;
			}
		}

	}

	void MaterialParameters_Serialize(const MaterialParameters* material, YAML::Emitter& out)
	{
		VXM_PROFILE_FUNCTION();
		if(material == nullptr) return;
		const MaterialParameters& mat = *material;
		out << KEYVAL("AlphaMode", mat.AlphaMode);
		out << KEYVAL("AlphaCutoff", mat.AlphaCutoff);
		out << KEYVAL("DoubleSided", mat.DoubleSided);
		out << KEYVAL("GammaCorrection", mat.GammaCorrection);

		{
			auto& metal = mat.PbrMetallicRoughness;
			out << KEYVAL("BaseColor", metal.BaseColorFactor);
			out << KEYVAL("BaseColorIndex", metal.BaseColorTexture.Index);
			out << KEYVAL("BaseColorTexCoord", metal.BaseColorTexture.TexCoord);

			out << KEYVAL("MetallicFactor", metal.MetallicFactor);
			out << KEYVAL("RoughnessFactor", metal.RoughnessFactor);
			out << KEYVAL("MetallicRoughnessIndex", metal.MetallicRoughnessTexture.Index);
			out << KEYVAL("MetallicRoughnessTexCoord", metal.MetallicRoughnessTexture.TexCoord);
		}

		{
			auto& normal = mat.NormalTexture;
			out << KEYVAL("NormalMapIndex", normal.Index);
			out << KEYVAL("NormalMapTexCoord", normal.TexCoord);
			out << KEYVAL("NormalMapScale", normal.Scale);
		}

		{
			auto& occlusion = mat.OcclusionTexture;
			out << KEYVAL("OcclusionTextureIndex", occlusion.Index);
			out << KEYVAL("OcclusionTextureTexCoord", occlusion.TexCoord);
			out << KEYVAL("OcclusionTextureStrength", occlusion.Strength);
		}

		{
			auto& emissive = mat.EmissiveTexture;
			out << KEYVAL("EmissiveFactor", mat.EmissiveFactor);
			out << KEYVAL("EmissiveTextureIndex", emissive.Index);
			out << KEYVAL("EmissiveTextureTexCoord", emissive.TexCoord);
		}
	}

	bool MaterialParameters_ImGui(MaterialParameters* material)
	{
		VXM_PROFILE_FUNCTION();
		if(material == nullptr) return false;
		bool changed = false;
		static const char* alphaModeNames[3] = {"Opaque", "Mask", "Blend"};
		MaterialParameters& mat = *material;

		// ===== Material params =====
		changed |= ImGui::Combo("Alpha Mode", &mat.AlphaMode, alphaModeNames, IM_ARRAYSIZE(alphaModeNames));
		ImGui::BeginDisabled(mat.AlphaMode != AlphaMode::Mask);
		changed |= ImGui::DragFloat("Alpha Cutoff", &mat.AlphaCutoff, 0.01f, 0.0f, 1.0f);
		ImGui::EndDisabled();
		bool doubleSided = mat.DoubleSided; if(ImGui::Checkbox("Double Sided", &doubleSided)) { mat.DoubleSided = doubleSided; changed = true; }
		bool gammaCorrection = mat.GammaCorrection; if(ImGui::Checkbox("Gamma Correction", &gammaCorrection)) { mat.GammaCorrection = gammaCorrection; changed = true; }

		// ===== MetallicRoughtness PbrMetallicRoughness =====
		if(ImGui::CollapsingHeader("Metallic Roughtness"))
		{
			auto& metal = mat.PbrMetallicRoughness;
			changed |= ImGui::ColorEdit4("Base Color", Math::ValuePtr(metal.BaseColorFactor));
//			ImGui::BeginDisabled(true);
			changed |= ImGui::InputInt("Base Index##BaseColor", &metal.BaseColorTexture.Index, 1, -1, 32);
			changed |= ImGui::InputInt("Base TexCoord##BaseColor", &metal.BaseColorTexture.TexCoord);
//			ImGui::EndDisabled();

			ImGui::Separator();

			changed |= ImGui::DragFloat("Metallic Factor", &metal.MetallicFactor, 0.01f, 0.0f, 1.0f);
			changed |= ImGui::DragFloat("Roughness Factor", &metal.RoughnessFactor, 0.01f, 0.0f, 1.0f);
//			ImGui::BeginDisabled(true);
			changed |= ImGui::InputInt("Metallic Index##MetallicRoughness", &metal.MetallicRoughnessTexture.Index, 1, -1, 32);
			changed |= ImGui::InputInt("Metallic TexCoord##MetallicRoughness", &metal.MetallicRoughnessTexture.TexCoord);
//			ImGui::EndDisabled();
		}
		// ===== NormalTextureInfo NormalTexture =====
		if(ImGui::CollapsingHeader("Normal Texture"))
		{
			auto& normal = mat.NormalTexture;
			changed |= ImGui::InputInt("Index##NormalMap", &normal.Index, 1, -1, 32);
			changed |= ImGui::InputInt("TexCoord##NormalMap", &normal.TexCoord);
			changed |= ImGui::DragFloat("Scale##NormalMap", &normal.Scale, 0.01f);
		}

		// ===== OcclusionTextureInfo OcclusionTexture =====
		if(ImGui::CollapsingHeader("Occlusion Texture"))
		{
			auto& occlusion = mat.OcclusionTexture;
			changed |= ImGui::InputInt("Index##OcclusionTexture", &occlusion.Index, 1, -1, 32);
			changed |= ImGui::InputInt("TexCoord##OcclusionTexture", &occlusion.TexCoord);
			changed |= ImGui::DragFloat("Strength##OcclusionTexture", &occlusion.Strength, 0.01f);
		}

		// ===== TextureInfo EmissiveTexture =====
		if(ImGui::CollapsingHeader("Emissive"))
		{
			auto& emissive = mat.EmissiveTexture;
			changed |= ImGui::ColorEdit3("Emissive Factor", Math::ValuePtr(mat.EmissiveFactor));
			changed |= ImGui::InputInt("Index##EmissiveTexture", &emissive.Index, 1, -1, 32);
			changed |= ImGui::InputInt("TexCoord##EmissiveTexture", &emissive.TexCoord);
		}

		return changed;
	}

} // Voxymore
// Core