//
// Created by ianpo on 01/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Serializable.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"


namespace Voxymore::Core {

	enum AlphaMode : int
	{
		Opaque = 0,
		Mask,
		Blend,
	};

	struct 	TextureInfo
	{
		int Index = -1;
		int TexCoord = 0;
		int Padding[2];
	};

	struct NormalTextureInfo
	{
		int Index = -1;
		int TexCoord = 0;
		float Scale = 1.0f;
		int Padding[1];
	};

	struct OcclusionTextureInfo
	{
		int Index = -1;
		int TexCoord = 0;
		float Strength = 1.0f;
		int Padding[1];
	};

	struct MetallicRoughtness
	{
		glm::vec4 BaseColorFactor = {1, 1, 1, 1};
		TextureInfo BaseColorTexture;
		TextureInfo MetallicRoughnessTexture;
		float MetallicFactor = 1.0f;
		float RoughnessFactor = 1.0f;
		int Padding[2];
	};

	struct MaterialParameters
	{
		MetallicRoughtness PbrMetallicRoughness;
		NormalTextureInfo NormalTexture;
		OcclusionTextureInfo OcclusionTexture;
		TextureInfo EmissiveTexture;
		glm::vec4 EmissiveFactor = {0,0,0,1};
		int AlphaMode = AlphaMode::Opaque;
		float AlphaCutoff = 0.5f;
		int DoubleSided = false;
		int GammaCorrection = false;
	};

	class Material : public Serializable, public Asset
	{
	public:
		VXM_IMPLEMENT_ASSET(AssetType::Material);
	private://To Serialize
		//TODO: Replace with a UUID.
		std::string m_MaterialName = "";
		MaterialParameters m_Parameters;
		std::array<Texture2DField, 32> m_Textures;
		bool m_IsForward = true;
		ShaderField m_Shader;
	public:
		Material();
		Material(const std::string& name);
		Material(const MaterialParameters& parameters);
		Material(const ShaderField& shader);
		Material(const std::string& name, const MaterialParameters& parameters);
		Material(const ShaderField& shader, const MaterialParameters& parameters);
		Material(const std::string& name, const ShaderField& shader, const MaterialParameters& parameters);
		~Material();

		void Bind(bool bindShader = true) const;
		void Unbind() const;

		void ChangeShader(AssetHandle shaderHandle);
		void ChangeShader(const Ref<Shader>& shader);
		const std::string& GetMaterialName() const;
		inline AssetHandle GetShaderHandle() const {return m_Shader.GetHandle();}
		inline Ref<Shader> GetShader() const {return m_Shader.GetAsset();}
		void SetMaterialName(const std::string& name);

	public:
		const MaterialParameters& GetMaterialsParameters() const;
		MaterialParameters& GetMaterialsParameters();

		void SetTexture(Texture2DField texture, int binding);
		void UnsetTexture(int binding);
	public:
		virtual void Deserialize(YAML::Node& node) override;
		virtual void Serialize(YAML::Emitter&out) const override;
		virtual bool OnImGui() override;
	private:
		void ResetShader();
	};

	using MaterialField = AssetField<Material>;

	static void MaterialParameters_Deserialize(MaterialParameters* material, YAML::Node& node);
	static void MaterialParameters_Serialize(const MaterialParameters* material, YAML::Emitter& emitter);
	static bool MaterialParameters_ImGui(MaterialParameters* material);

} // Voxymore
// Core
