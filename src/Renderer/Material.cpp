//
// Created by ianpo on 01/08/2023.
//

#include "Voxymore/Renderer/Material.hpp"

#define SerializeSerializable(Emitter, Param) Emitter << KEYVAL(#Param, YAML::BeginMap); \
Param.Serialize(Emitter); \
Emitter << YAML::EndMap

#define SerializeSerializableIf(Condition, Emitter, Param) if(Condition) {SerializeSerializable(Emitter, Param);}

#define DeserializeSerializableIf(Condition, node, Param) auto VXM_COMBINE(Param, Node) = node[#Param]; \
Condition = VXM_COMBINE(Param, Node).IsDefined(); \
if(Condition) Param.Deserialize(VXM_COMBINE(Param, Node))

#define DeserializeSerializable(node, Param) auto VXM_COMBINE(Param, Node) = node[#Param]; \
Param.Deserialize(VXM_COMBINE(Param, Node))

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if(!node.IsSequence() || node.size() != 2) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if(!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if(!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::quat& rhs)
		{
			if(!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Voxymore::Core {
//	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec2& v)
//	{
//		out << YAML::Flow;
//		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
//		return out;
//	}
//	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& v)
//	{
//		out << YAML::Flow;
//		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
//		return out;
//	}
//	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& v)
//	{
//		out << YAML::Flow;
//		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
//		return out;
//	}
//	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::quat& v)
//	{
//		out << YAML::Flow;
//		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
//		return out;
//	}

	void MaterialParameters::Deserialize(YAML::Node &node)
	{
		DeserializeSerializable(node, PbrMetallicRoughness);
		DeserializeSerializableIf(HasNormalTexture, node, NormalTexture);
		DeserializeSerializableIf(HasOcclusionTexture, node, OcclusionTexture);
		DeserializeSerializableIf(HasEmissiveTexture, node, EmissiveTexture);

		EmissiveFactor = node["EmissiveFactor"].as<glm::vec3>();
		AlphaMode = (::Voxymore::Core::AlphaMode)node["AlphaMode"].as<int>();
		AlphaCutoff = node["AlphaCutoff"].as<float>();
		DoubleSided = node["DoubleSided"].as<bool>();
	}

	void MaterialParameters::Serialize(YAML::Emitter &emitter) const
	{
		SerializeSerializable(emitter, PbrMetallicRoughness);
		SerializeSerializableIf(HasNormalTexture, emitter, NormalTexture);
		SerializeSerializableIf(HasOcclusionTexture, emitter, OcclusionTexture);
		SerializeSerializableIf(HasEmissiveTexture, emitter, EmissiveTexture);

		emitter << KEYVAL("EmissiveFactor", EmissiveFactor);
		emitter << KEYVAL("AlphaMode", AlphaMode);
		emitter << KEYVAL("AlphaCutoff", AlphaCutoff);
		emitter << KEYVAL("DoubleSided", DoubleSided);
	}

	void MaterialParameters::SetAlphaMode(const std::string &alphaMode)
	{
		if(alphaMode == "OPAQUE") { AlphaMode = AlphaMode::Opaque; }
		else if(alphaMode == "MASK") { AlphaMode = AlphaMode::Mask; }
		else if(alphaMode == "BLEND") { AlphaMode = AlphaMode::Blend; }
		else { VXM_CORE_WARNING("The alpha mode '{0}' is unknown.", alphaMode); }
	}

	void MetallicRoughtness::Serialize(YAML::Emitter &emitter) const
	{
		emitter << KEYVAL("BaseColorFactor", BaseColorFactor);
		SerializeSerializableIf(HasBaseColorTexture, emitter, BaseColorTexture);
		emitter << KEYVAL("MetallicFactor", MetallicFactor);
		emitter << KEYVAL("RoughnessFactor", RoughnessFactor);
		SerializeSerializableIf(HasMetallicRoughnessTexture, emitter, MetallicRoughnessTexture);
	}

	void MetallicRoughtness::Deserialize(YAML::Node &node)
	{
		BaseColorFactor = node["BaseColorFactor"].as<glm::vec4>();
		DeserializeSerializableIf(HasBaseColorTexture, node, BaseColorTexture);
		MetallicFactor = node["MetallicFactor"].as<float>();
		RoughnessFactor = node["RoughnessFactor"].as<float>();
		DeserializeSerializableIf(HasMetallicRoughnessTexture, node, MetallicRoughnessTexture);
	}

	void TextureInfo::Serialize(YAML::Emitter &emitter) const
	{
		emitter << KEYVAL("Index", Index);
		emitter << KEYVAL("TexCoord", TexCoord);
	}

	void TextureInfo::Deserialize(YAML::Node &node)
	{
		Index = node["Index"].as<int>();
		TexCoord = node["TexCoord"].as<int>();
	}

	void NormalTextureInfo::Serialize(YAML::Emitter &emitter) const
	{
		emitter << KEYVAL("Index", Index);
		emitter << KEYVAL("TexCoord", TexCoord);
		emitter << KEYVAL("Scale", Scale);
	}

	void NormalTextureInfo::Deserialize(YAML::Node &node)
	{
		Index = node["Index"].as<int>();
		TexCoord = node["TexCoord"].as<int>();
		Scale = node["Scale"].as<float>();
	}

	void OcclusionTextureInfo::Serialize(YAML::Emitter &emitter) const
	{
		emitter << KEYVAL("Index", Index);
		emitter << KEYVAL("TexCoord", TexCoord);
		emitter << KEYVAL("Strength", Strength);
	}

	void OcclusionTextureInfo::Deserialize(YAML::Node &node)
	{
		Index = node["Index"].as<int>();
		TexCoord = node["TexCoord"].as<int>();
		Strength = node["Strength"].as<float>();
	}

	Material::Material(uint32_t binding)
	{
		m_UniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), binding);
	}

	Material::Material(const std::string &shaderName, uint32_t binding) : m_ShaderName(shaderName)
	{
		VXM_PROFILE_FUNCTION();
		m_UniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), binding);
		LoadShader();
	}

	Material::Material(const std::string& shaderName, const MaterialParameters& parameters, uint32_t binding) : m_ShaderName(shaderName), m_Parameters(parameters)
	{
		VXM_PROFILE_FUNCTION();
		m_UniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), binding);
		LoadShader();
	}

	Material::Material(const Ref<Shader> &shader, uint32_t binding) : m_ShaderName(shader->GetName()), m_Shader(shader)
	{
		VXM_PROFILE_FUNCTION();
		m_UniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), binding);
	}

	Material::Material(const Ref<Shader>& shader, const MaterialParameters& parameters, uint32_t binding) : m_ShaderName(shader->GetName()), m_Shader(shader), m_Parameters(parameters)
	{
		VXM_PROFILE_FUNCTION();
		m_UniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), binding);
	}

	Material::~Material()
	{
	}

	void Material::Bind() const
	{
		m_UniformBuffer->SetData(&m_Parameters, sizeof(MaterialParameters));
//		for (uint32_t i = 0; i < m_Textures.size(); ++i)
//		{
//			if(m_Textures[i] != nullptr) LoadTexture(i);
//		}
		m_Shader->Bind();
	}

	void Material::Unbind() const
	{
		m_Shader->Unbind();
	}

	void Material::ChangeShader(const std::string &shaderName)
	{
		ResetShader();
		m_ShaderName = shaderName;
		LoadShader();
	}

//	void Material::LoadTexture(uint32_t index) const
//	{
//		VXM_CORE_ASSERT(index >= 0 && index < m_Textures.size(), "The index {0} is not between [0, {1}[", index, m_Textures.size());
//		VXM_CORE_ASSERT(m_Textures[index] != nullptr, "The texture {0} doesn't exist.", index);
//
//		m_Textures[index]->Bind(index);
//	}

	const std::string &Material::GetMaterialName() const
	{
		return m_MaterialName;
	}

	const std::string &Material::GetShaderName() const
	{
		return m_ShaderName;
	}

	void Material::Serialize(YAML::Emitter &emitter) const
	{
		emitter << KEYVAL("MaterialName", m_MaterialName);
		emitter << KEYVAL("ShaderName", m_ShaderName);
		emitter << KEYVAL("Parameters", YAML::BeginMap);
		m_Parameters.Serialize(emitter);
		emitter << YAML::EndMap;
//		emitter << KEYVAL("Textures", YAML::BeginMap);
//		for (int i = 0; i < m_TexturesPath.size(); ++i) {
//			if(m_TexturesPath[i].has_value()) emitter << KEYVAL(i, m_TexturesPath[i].value());
//		}
//		emitter << YAML::EndMap;
	}
	void Material::Deserialize(YAML::Node &node)
	{
		m_MaterialName = node["MaterialName"].as<std::string>();
		m_ShaderName = node["ShaderName"].as<std::string>();
		auto ParameterNode = node["Parameters"];
		m_Parameters.Deserialize(ParameterNode);
//		auto TextureNode = node["Textures"];
//		if(TextureNode.IsDefined())
//		{
//			for (int i = 0; i < m_TexturesPath.size(); ++i)
//			{
//				if(TextureNode[i].IsDefined())
//				{
//					m_TexturesPath[i] = TextureNode[i].as<Path>();
//				}
//				else
//				{
//					m_TexturesPath[i] = {};
//				}
//			}
//		}
//		else
//		{
//			for (auto &tex: m_TexturesPath) tex = {};
//		}
		LoadShader();
	}

	MaterialLibrary* MaterialLibrary::s_Instance = nullptr;
	MaterialLibrary& MaterialLibrary::GetInstance()
	{
		if(s_Instance == nullptr) s_Instance = new Voxymore::Core::MaterialLibrary();
		return *s_Instance;
	}

	void MaterialLibrary::Add(const Ref<Material>& material)
	{
		m_Materials[material->GetMaterialName()] = material;
	}

	void MaterialLibrary::Add(const std::string& name, const Ref<Material>& material)
	{
		m_Materials[name] = material;
	}

	Ref<Material> MaterialLibrary::Get(const std::string& name)
	{
		VXM_CORE_ASSERT(Exists(name), "The material '{0}' doesn't exist.", name);
		return m_Materials[name];
	}

	bool MaterialLibrary::Exists(const std::string& name) const
	{
		return m_Materials.contains(name);
	}
	void MaterialLibrary::Deserialize(YAML::Node &node)
	{
		m_Materials.clear();
		for(YAML::iterator it = node.begin(); it != node.end(); ++it)
		{
			std::string name = it->first.as<std::string>();
			Ref<Material> material = CreateRef<Material>();
			material->Deserialize(it->second);
			m_Materials[name] = material;
		}
	}
	void MaterialLibrary::Serialize(YAML::Emitter &emitter) const
	{
		for (auto&& [name, material]: m_Materials)
		{
			emitter << KEYVAL(name, YAML::BeginMap);
			material->Serialize(emitter);
			emitter << YAML::EndMap;
		}
	}
	Ref<Material> MaterialLibrary::GetOrCreate(const std::string &materialName, const std::string &shaderName)
	{
		if(!Exists(materialName))
		{
			m_Materials[materialName] = CreateRef<Material>(shaderName);
			m_Materials[materialName]->SetMaterialName(materialName);
		}
		return m_Materials[materialName];
	}

	void Material::SetMaterialName(const std::string &name)
	{
		m_MaterialName = name;
	}

	void Material::ResetShader()
	{
		m_Shader = nullptr;
	}

	void Material::LoadShader()
	{
		VXM_CORE_ASSERT(ShaderLibrary::GetInstance().Exists(m_ShaderName), "The shader {0} doesn't exist...", m_ShaderName);
		m_Shader = ShaderLibrary::GetInstance().Get(m_ShaderName);
	}

} // Voxymore
// Core