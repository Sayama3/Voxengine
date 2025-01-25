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
		return handle && (m_AssetRegistry.contains(handle) || m_MemoryAssets.contains(handle));
	}

	bool EditorAssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		return m_LoadedAssets.contains(handle) || m_MemoryAssets.contains(handle);
	}

	bool EditorAssetManager::IsAssetImported(const Path& path, AssetHandle* handle/* = nullptr*/) const
	{
		auto it = std::find_if(m_AssetRegistry.begin(), m_AssetRegistry.end(), [&path](std::pair<UUID, AssetMetadata> assets) {
			return assets.second.FilePath.equivalent(path);
		});

		if(handle && it != m_AssetRegistry.end()) {
			*handle = it->first;
		}

		return it != m_AssetRegistry.end();
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
			asset = GetLoadedAsset(handle);
		}
		else
		{
			const auto& metadata = GetMetadata(handle);
			asset = AssetImporter::ImportAsset(metadata);
			if(asset) {
				m_LoadedAssets.insert({handle, asset});
			} else {
				VXM_CORE_ERROR("Could not load the asset {0}", handle.string());
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
			return GetLoadedAsset(handle)->GetType();
		}
		else
		{
			const auto& metadata = GetMetadata(handle);
			return metadata.Type;
		}
	}

	Ref<Asset> EditorAssetManager::ImportAsset(const Path& assetPath, AssetType hint/* = AssetType::None*/)
	{
		if(!FileSystem::Exist(assetPath) || AssetImporter::GetAssetType(assetPath) == AssetType::None) {
			//				VXM_CORE_WARN("The path '{0}' either doesn't exist or cannot be converted.", assetPath.string());
			return nullptr;
		}

		AssetMetadata metadata;

		metadata.FilePath = assetPath;
		metadata.Type = AssetImporter::HasAssetType(assetPath, hint) ? hint : AssetImporter::GetAssetType(assetPath);
		auto asset = AssetImporter::ImportAsset(metadata);

		if(asset)
		{
			m_LoadedAssets[metadata.Handle] = asset;
			m_AssetRegistry[metadata.Handle] = metadata;
			SerializeAssetRegistry();
		}

		return asset;
	}

	Ref<Asset> EditorAssetManager::GetOrCreateAsset(const Path& assetPath, AssetType hint/* = AssetType::None*/)
	{
		VXM_PROFILE_FUNCTION();

		Ref<Asset> asset = nullptr;

		const auto it = std::find_if(m_AssetRegistry.begin(), m_AssetRegistry.end(), [&assetPath](const std::pair<UUID, AssetMetadata>& assets) {
		  return assets.second.FilePath.equivalent(assetPath);
		});

		if(it == m_AssetRegistry.end()) {
			asset = ImportAsset(assetPath);
		} else {
			asset = GetAsset(it->first);
			if(hint != AssetType::None && hint != asset->GetType()) {
				VXM_CORE_WARN("Asset '{}'({}) has type '{}', but the expected type is '{}'.", assetPath.string(), it->first.string(), AssetTypeToString(asset->GetType()), AssetTypeToString(hint));
			}
		}

		return asset;
	}

	Path EditorAssetManager::GetFilePath(const AssetHandle handle) const
	{
		return GetMetadata(handle).FilePath;
	}

	const AssetMetadata& EditorAssetManager::GetMetadata(const AssetHandle handle) const
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

	void EditorAssetManager::SetPath(AssetHandle handle, Path newPath)
	{
		VXM_PROFILE_FUNCTION();

		auto it = m_AssetRegistry.find(handle);
		if(it != m_AssetRegistry.end()) {
			it->second.FilePath = newPath;
			SerializeAssetRegistry();
		}
	}

	void EditorAssetManager::SerializeAssetRegistry()
	{
		VXM_PROFILE_FUNCTION();
		auto assetRegistryPath = Project::GetAssetRegistryPath();
		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << KEYVAL("Type", "AssetRegistry");
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

	bool EditorAssetManager::AddMemoryAsset(Ref<Asset> asset)
	{
		VXM_PROFILE_FUNCTION();
		if(!asset) return false;
		m_MemoryAssets.emplace(asset->Handle, asset);
		return true;
	}

	bool EditorAssetManager::AddAsset(Ref<Asset> asset, const Path& path)
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

	bool EditorAssetManager::SaveMemoryAsset(AssetHandle handle, const Path& path)
	{
		VXM_PROFILE_FUNCTION();
		auto it = m_MemoryAssets.find(handle);
		if(it == m_MemoryAssets.end()) return false;

		Ref<Asset> asset = it->second;
		if(!asset) return false;

		AssetMetadata metadata;
		metadata.Handle = handle;
		metadata.FilePath = path;
		metadata.Type = asset->GetType();

		m_MemoryAssets.erase(it);
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

		auto memory_it = m_MemoryAssets.find(handle);
		if(memory_it != m_MemoryAssets.end()) {
			m_MemoryAssets.erase(memory_it);
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

		auto memory_it = m_MemoryAssets.find(handle);
		if(memory_it != m_MemoryAssets.end()) {
			m_MemoryAssets.erase(memory_it);
		}
	}

	Ref<Asset> EditorAssetManager::GetLoadedAsset(AssetHandle handle) const
	{
		VXM_PROFILE_FUNCTION();
		if (m_LoadedAssets.contains(handle)) {
			return m_LoadedAssets.at(handle);
		} else if(m_MemoryAssets.contains(handle)) {
			return m_MemoryAssets.at(handle);
		}
		return nullptr;
	}
} // namespace Voxymore::Core
