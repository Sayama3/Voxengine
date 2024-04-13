//
// Created by ianpo on 09/04/2024.
//

#include "Voxymore/Assets/EditorAssetManager.hpp"
#include "Voxymore/Assets/AssetImporter.hpp"
#include "Voxymore/Project/Project.hpp"
#include "Voxymore/Core/Core.hpp"
#include <algorithm>


namespace Voxymore::Core
{
	bool EditorAssetManager::IsAssetHandleValid(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		return handle != 0 && m_AssetRegistry.contains(handle);
	}

	bool EditorAssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		return m_LoadedAssets.contains(handle);
	}

	Ref<Asset> EditorAssetManager::GetAsset(AssetHandle handle)
	{
		VXM_PROFILE_FUNCTION();
		if(!IsAssetHandleValid(handle)) {
			return nullptr;
		}

		Ref<Asset> asset = nullptr;

		if(IsAssetLoaded(handle))
		{
			asset = m_LoadedAssets.at(handle);
		}
		else
		{
			const auto& metadata = GetMetadata(handle);
			asset = AssetImporter::ImportAsset(metadata);
			if(asset) {
				m_LoadedAssets.insert({handle, asset});
			} else {
				VXM_CORE_ERROR("Could not load the asset {0}", handle);
				//TODO: use this version :
				// VXM_CORE_ERROR("Could not load the asset {0} ({1})", handle, metadata);
			}
		}

		return asset;
	}

	AssetType EditorAssetManager::GetAssetType(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		if(!IsAssetHandleValid(handle)) {
			return AssetType::None;
		}

		if(IsAssetLoaded(handle))
		{
			return m_LoadedAssets.at(handle)->GetType();
		}
		else
		{
			const auto& metadata = GetMetadata(handle);
			return metadata.Type;
		}
	}

	Ref<Asset> EditorAssetManager::ImportAsset(Path assetPath)
	{
		if(!FileSystem::Exist(assetPath) || AssetImporter::GetAssetType(assetPath) == AssetType::None) {
			//				VXM_CORE_WARN("The path '{0}' either doesn't exist or cannot be converted.", assetPath.string());
			return nullptr;
		}

		AssetMetadata metadata;

		metadata.FilePath = assetPath;
		metadata.Type = AssetImporter::GetAssetType(assetPath);
		auto asset = AssetImporter::ImportAsset(metadata);

		if(asset)
		{
			m_LoadedAssets[metadata.Handle] = asset;
			m_AssetRegistry[metadata.Handle] = metadata;
			SerializeAssetRegistry();
		}

		return asset;
	}

	Ref<Asset> EditorAssetManager::GetOrCreateAsset(Path assetPath)
	{
		VXM_PROFILE_FUNCTION();

		Ref<Asset> asset = nullptr;

		auto it = std::find_if(m_AssetRegistry.begin(), m_AssetRegistry.end(), [&assetPath](std::pair<UUID, AssetMetadata> assets) {
			return assets.second.FilePath == assetPath;
		});

		AssetMetadata metadata;

		if(it == m_AssetRegistry.end()) {
			asset = ImportAsset(assetPath);

		} else {
			asset = GetAsset(it->first);
		}

		return asset;
	}


	Path EditorAssetManager::GetFilePath(AssetHandle handle) const
	{
		return GetMetadata(handle).FilePath;
	}


	const AssetMetadata& EditorAssetManager::GetMetadata(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		static AssetMetadata s_NullMetadata;
		auto it = m_AssetRegistry.find(handle);
		if(it == m_AssetRegistry.end()) {
			return s_NullMetadata;
		}
		return it->second;
	}

	AssetMetadata EditorAssetManager::GetMetadata(const Path& assetPath) const
	{
		VXM_PROFILE_FUNCTION();
		static AssetMetadata s_NullMetadata;
		auto it = std::find_if(m_AssetRegistry.begin(), m_AssetRegistry.end(), [&assetPath](std::pair<UUID, AssetMetadata> assets) {
		  return assets.second.FilePath == assetPath;
		});

		return it != m_AssetRegistry.end() ? it->second : s_NullMetadata;
	}

	void EditorAssetManager::SerializeAssetRegistry()
	{
		VXM_PROFILE_FUNCTION();
		auto assetRegistryPath = Project::GetAssetRegistryPath();
		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("AssetRegistry", YAML::BeginMap);
			{
				out << KEYVAL("Assets", YAML::BeginSeq);
				{
					for(auto&&[handle, metadata] : m_AssetRegistry)
					{
						out << YAML::BeginMap;
						{
							out << KEYVAL("Handle", metadata.Handle);
							out << KEYVAL("FilePath", metadata.FilePath);
							out << KEYVAL("Type", AssetTypeToString(metadata.Type));
							out << YAML::EndMap;
						}
					}
					out << YAML::EndSeq;
				}
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		out << KEYVAL("Type", "AssetRegistry");

		FileSystem::WriteYamlFile(Project::GetAssetRegistryPath(), out);
	}

	void EditorAssetManager::DeserializeAssetRegistry()
	{
		VXM_PROFILE_FUNCTION();
		auto filepath = Project::GetAssetRegistryPath();
		if(!FileSystem::Exist(filepath)) {
			VXM_CORE_WARN("The AssetRegistry at path '{0}' doesn't exist.", filepath.string());
			return;
		}

		YAML::Node data;
		std::ifstream ifstream(filepath);
		std::stringstream  stringstream;
		stringstream << ifstream.rdbuf();
		try
		{
			data = YAML::Load(stringstream.str());
		}
		catch(YAML::ParserException& e)
		{
			VXM_CORE_CRITICAL("Failed to load Asset Registry '{0}'.\n\t{1}", filepath.string(), e.what());
			return;
		}

		auto assetRegistryNode = data["AssetRegistry"];
		if(!assetRegistryNode) return;

		auto assetsNode = assetRegistryNode["Assets"];
		if(!assetsNode) return;

		for (auto assetNode: assetsNode) {
			AssetMetadata metadata;
			metadata.Handle = assetNode["Handle"].as<AssetHandle>();
			metadata.FilePath = assetNode["FilePath"].as<Path>();
			metadata.Type = AssetTypeFromString(assetNode["Type"].as<std::string>());
			m_AssetRegistry[metadata.Handle] = metadata;
		}
	}

	bool EditorAssetManager::AddAsset(Ref<Asset> asset)
	{
		VXM_PROFILE_FUNCTION();
		if(!asset) return false;
		AssetMetadata metadata;
		metadata.Handle = asset->Handle;
		metadata.FilePath = {FileSource::Cache, "MemoryAssets/"};
		metadata.FilePath.path += metadata.Handle.string() + ".vxm_memory";
		metadata.Type = asset->GetType();
		m_AssetRegistry.emplace(metadata.Handle, metadata);
		m_LoadedAssets.emplace(metadata.Handle, asset);
		SerializeAssetRegistry();
		return true;
	}
	bool EditorAssetManager::AddAsset(Ref<Asset> asset, Path path)
	{
		VXM_PROFILE_FUNCTION();
		if(!asset) return false;
		AssetMetadata metadata;
		metadata.Handle = asset->Handle;
		metadata.FilePath = path;
		metadata.Type = asset->GetType();
		m_AssetRegistry.emplace(metadata.Handle, metadata);
		m_LoadedAssets.emplace(metadata.Handle, asset);
		SerializeAssetRegistry();
		return true;
	}
	void EditorAssetManager::RemoveAsset(AssetHandle handle)
	{
		VXM_PROFILE_FUNCTION();
		if(!IsAssetHandleValid(handle)) return;

		auto loaded_it = m_LoadedAssets.find(handle);
		if(loaded_it != m_LoadedAssets.end()) {
			m_LoadedAssets.erase(loaded_it);
		}

		auto registry_it = m_AssetRegistry.find(handle);
		if(registry_it != m_AssetRegistry.end()) {
			m_AssetRegistry.erase(registry_it);
		}

		SerializeAssetRegistry();
	}

	void EditorAssetManager::UnloadAsset(AssetHandle handle)
	{
		VXM_PROFILE_FUNCTION();
		if(!IsAssetHandleValid(handle)) return;

		auto loaded_it = m_LoadedAssets.find(handle);
		if(loaded_it != m_LoadedAssets.end()) {
			m_LoadedAssets.erase(loaded_it);
		}
	}
} // namespace Voxymore::Core
