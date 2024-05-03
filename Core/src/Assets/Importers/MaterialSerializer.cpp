//
// Created by ianpo on 12/04/2024.
//

#include "Voxymore/Assets/Importers/MaterialSerializer.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/YamlHelper.hpp"


namespace Voxymore::Core
{

	bool MaterialSerializer::IsMaterial(const std::filesystem::path &path)
	{
		VXM_PROFILE_FUNCTION();
		std::string extension = path.extension().string();

		std::transform(extension.begin(), extension.end(), extension.begin(), [](char c){ return std::tolower(c);});

		return path.has_extension() && extension == MaterialExtension;
	}

	Ref<Asset> MaterialSerializer::ImportMaterial(const AssetMetadata &metadata)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsMaterial(metadata.FilePath), "The asset '{0}' is not a material", metadata.FilePath.string());

		YAML::Node root = FileSystem::ReadFileAsYAML(metadata.FilePath);
		YAML::Node node = root["Material"];
		if(!node) return nullptr;

		Ref<Material> material = CreateRef<Material>();
		material->Deserialize(node);

		return material;
	}

	void MaterialSerializer::ExportEditorMaterial(const AssetMetadata& metadata, Ref<Material> material)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(IsMaterial(metadata.FilePath), "The asset '{0}' is not a material", metadata.FilePath.string());


		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("Material", YAML::BeginMap);
			{
				material->Serialize(out);
			}
			out << YAML::EndMap;
		}
		out << YAML::EndMap;

		FileSystem::WriteYamlFile(metadata.FilePath, out);
	}
} // namespace Voxymore::Core
