//
// Created by ianpo on 01/08/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Serializable.hpp"
#include "Voxymore/Core/YamlHelper.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Buffer.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/Texture.hpp"


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

		//		void Deserialize(YAML::Node& node);
		//		void Serialize(YAML::Emitter& emitter) const;
	};

	struct NormalTextureInfo
	{
		int Index = -1;
		int TexCoord = 0;
		float Scale = 1.0f;

		//		void Deserialize(YAML::Node& node);
		//		void Serialize(YAML::Emitter& emitter) const;
	};

	struct OcclusionTextureInfo
	{
		int Index = -1;
		int TexCoord = 0;
		float Strength = 1.0f;

		//		void Deserialize(YAML::Node& node);
		//		void Serialize(YAML::Emitter& emitter) const;
	};

	struct MetallicRoughtness
	{
		glm::vec4 BaseColorFactor = {1, 1, 1, 1};
		TextureInfo BaseColorTexture;
		float MetallicFactor = 1.0f;
		float RoughnessFactor = 1.0f;
		TextureInfo MetallicRoughnessTexture;

		//		void Deserialize(YAML::Node& node);
		//		void Serialize(YAML::Emitter& emitter) const;
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
	};



	class Material : public Serializable
	{
	private://To Serialize
		//TODO: Replace with a UUID.
		std::string m_MaterialName = "";
		std::string m_ShaderName = "Default";
		MaterialParameters m_Parameters;
		std::array<Ref<Texture2D>, 32> m_Textures;
	private:
		Ref<Shader> m_Shader;
	public:
		Material();
		Material(const std::string& shaderName);
		Material(const std::string& shaderName, const MaterialParameters& parameters);
		Material(const Ref<Shader>& shader);
		Material(const Ref<Shader>& shader, const MaterialParameters& parameters);
		~Material();

		void Bind() const;
		void Unbind() const;

		void ChangeShader(const std::string& shaderName);
		void ChangeShader(Ref<Shader>& shader);
		const std::string& GetMaterialName() const;
		const std::string& GetShaderName() const;
		void SetMaterialName(const std::string& name);

	public:
		const MaterialParameters& GetMaterialsParameters() const;
		MaterialParameters& GetMaterialsParameters();

		void SetTexture(Ref<Texture2D> texture, int binding);
		void UnsetTexture(int binding);
	public:
		virtual void Deserialize(YAML::Node& node) override;
		virtual void Serialize(YAML::Emitter& emitter) const override;
		virtual bool OnImGui() override;
	private:
		void ResetShader();
		void LoadShader();
	};

	class MaterialLibrary : public Serializable
	{
	private:
		static MaterialLibrary* s_Instance;
		std::map<std::string, Ref<Material>> m_Materials;
	public:
		static MaterialLibrary& GetInstance();
		void Add(const Ref<Material>& material);
		void Add(const std::string& name, const Ref<Material>& material);
		Ref<Material> Get(const std::string& name);
		Ref<Material> GetOrCreate(const std::string &materialName, const std::string &shaderName = "");
		bool Exists(const std::string& name) const;
	public:
		virtual void Deserialize(YAML::Node& node) override;
		virtual void Serialize(YAML::Emitter& emitter) const override;
		virtual bool OnImGui() override;
	};

	static void MaterialParameters_Deserialize(MaterialParameters* material, YAML::Node& node);
	static void MaterialParameters_Serialize(const MaterialParameters* material, YAML::Emitter& emitter);
	static bool MaterialParameters_ImGui(MaterialParameters* material);

} // Voxymore
// Core
