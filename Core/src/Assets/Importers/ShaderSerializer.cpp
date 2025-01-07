//
// Created by ianpo on 12/04/2024.
//

#include "Voxymore/Assets/Importers/ShaderSerializer.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/YamlHelper.hpp"


namespace Voxymore::Core
{
	static std::vector<std::string> Extensions = {".glsl", ".shader", ".compute", ".comp", ".cs", ".vert", ".vs", ".tessco", ".tcs", ".tessev", ".tes", ".geom", ".gs", ".frag", ".fs", ".pixel", ".ps"};

	bool ShaderSerializer::IsShaderSource(const std::filesystem::path &path)
	{
		VXM_PROFILE_FUNCTION();
		std::string extension = path.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		return path.has_extension() && std::find(Extensions.begin(), Extensions.end(), extension) != Extensions.end();
	}

	Ref<Asset> ShaderSerializer::ImportShaderSource(const AssetMetadata &metadata)
	{
		VXM_PROFILE_FUNCTION();
		auto path = metadata.FilePath.GetFullPath();
		if(!IsShaderSource(path)) return nullptr;

		std::string extension = path.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		auto metaFilePath = metadata.FilePath;
		metaFilePath.path += ".meta";

		Ref<ShaderSource> source;
		ShaderType type = ShaderType::None;

		if(FileSystem::Exist(metaFilePath))
		{
			YAML::Node root = FileSystem::ReadFileAsYAML(metaFilePath);
			YAML::Node node = root["ShaderSource"]["ShaderType"];
			if (node)
			{
				type = Utils::ShaderTypeFromString(node.as<std::string>());
				source = CreateRef<ShaderSource>();
				source->Type = type;
			}
		}

		if(!source)
		{
			source = CreateRef<ShaderSource>();
			if (extension == ".compute" || extension == ".comp" || extension == ".cs") {
				type = ShaderType::COMPUTE_SHADER;
			}
			else if (extension == ".vert" || extension == ".vs") {
				type = ShaderType::VERTEX_SHADER;
			}
			else if (extension == ".tessco" || extension == ".tcs") {
				type = ShaderType::TESS_CONTROL_SHADER;
			}
			else if (extension == ".tessev" || extension == ".tes") {
				type = ShaderType::TESS_EVALUATION_SHADER;
			}
			else if (extension == ".geom" || extension == ".gs") {
				type = ShaderType::GEOMETRY_SHADER;
			}
			else if (extension == ".frag" || extension == ".fs" || extension == ".pixel" || extension == ".ps") {
				type = ShaderType::FRAGMENT_SHADER;
			}
			source->Type = type;
		}

		source->Source = FileSystem::ReadFileAsString(metadata.FilePath);
		return source;
	}

	void ShaderSerializer::ExportEditorShaderSource(const AssetMetadata &metadata, Ref<ShaderSource> source)
	{
		VXM_PROFILE_FUNCTION();
		auto metaFilePath = metadata.FilePath;
		metaFilePath.path += ".meta";

		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("ID", metadata.Handle);
			out << KEYVAL("Type", AssetTypeToString(AssetType::ShaderSource));
			out << KEYVAL("ShaderSource", YAML::BeginMap);
			{
				out << KEYVAL("ShaderType", Utils::ShaderTypeToString(source->Type));
			}
			out << YAML::EndMap;
		}
		out << YAML::EndMap;

		FileSystem::WriteYamlFile(metaFilePath, out);
	}


	bool ShaderSerializer::IsGraphicShader(const std::filesystem::path& path)
	{
		VXM_PROFILE_FUNCTION();
		std::string extension = path.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		return path.has_extension() && extension == ShaderExtension;
	}

	Ref<Asset> ShaderSerializer::ImportGraphicShader(const AssetMetadata& metadata)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsGraphicShader(metadata.FilePath), "The asset '{0}' is not a shader", metadata.FilePath.string());
		YAML::Node root = FileSystem::ReadFileAsYAML(metadata.FilePath);
		YAML::Node node = root["Shader"];
		if(!node) return nullptr;

		std::vector<ShaderSourceField> sources;
		sources.reserve(6);

		bool isForward = node["IsForward"].as<bool>(true);
		auto name = node["Name"].as<std::string>("NoName");
		for (const auto &sourceNode: node["Sources"]) {
			auto src = sourceNode.as<ShaderSourceField>();
			sources.push_back(src);
		}
		auto asset = Shader::Create(name, sources);
		asset->SetForward(isForward);
		return asset;
	}

	void ShaderSerializer::ExportEditorGraphicShader(const AssetMetadata& metadata, Ref<Shader> shader)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsGraphicShader(metadata.FilePath), "The asset '{0}' is not a shader", metadata.FilePath.string());

		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("Type", AssetTypeToString(AssetType::Shader));
			out << KEYVAL("Shader", YAML::BeginMap);
			{
				out << KEYVAL("Name", shader->GetName());
				out << KEYVAL("IsForward", shader->IsForward());
				out << KEYVAL("Sources", YAML::BeginSeq);
				{
					auto sources = shader->GetSources();
					for (const auto &source: sources) {
						out << source;
					}
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap;
		}
		out<< YAML::EndMap;

		FileSystem::WriteYamlFile(metadata.FilePath, out);
	}

	bool ShaderSerializer::IsComputeShader(const std::filesystem::path& path)
	{
		VXM_PROFILE_FUNCTION();
		std::string extension = path.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		return path.has_extension() && extension == ComputeShaderExtension;
	}

	void ShaderSerializer::ExportEditorComputeShader(const AssetMetadata& metadata, Ref<ComputeShader> shader)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsComputeShader(metadata.FilePath), "The asset '{0}' is not a compute shader", metadata.FilePath.string());

		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("Type", AssetTypeToString(AssetType::ComputeShader));
			out << KEYVAL("ComputeShader", YAML::BeginMap);
			{
				out << KEYVAL("Name", shader->GetName());
				out << KEYVAL("Source", shader->GetSource());
			}
			out << YAML::EndMap;
		}
		out<< YAML::EndMap;

		FileSystem::WriteYamlFile(metadata.FilePath, out);
	}

	Ref<Asset> ShaderSerializer::ImportComputeShader(const AssetMetadata& metadata)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsComputeShader(metadata.FilePath), "The asset '{0}' is not a compute shader", metadata.FilePath.string());
		YAML::Node root = FileSystem::ReadFileAsYAML(metadata.FilePath);
		YAML::Node node = root["ComputeShader"];
		if(!node) return nullptr;

		auto name = node["Name"].as<std::string>("NoName");
		auto source = node["Source"].as<ShaderSourceField>(ShaderSourceField{});

		auto asset = ComputeShader::Create(name, source);
		return asset;
	}

} // namespace Voxymore::Core
