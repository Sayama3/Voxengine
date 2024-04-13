//
// Created by ianpo on 09/04/2024.
//

#pragma once

#include "AssetManagerBase.hpp"
#include "AssetMetadata.hpp"
#include "AssetImporter.hpp"
#include <map>

namespace Voxymore::Core
{
	// ordered map is cool for debugging
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;
	using AssetRegistryIterator = std::map<AssetHandle, AssetMetadata>::iterator;

	class EditorAssetManager : public AssetManagerBase
	{
	public:
		[[nodiscard]] virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		[[nodiscard]] virtual bool IsAssetLoaded(AssetHandle handle) const override;
		[[nodiscard]] virtual AssetType GetAssetType(AssetHandle handle) const override;
		virtual Ref<Asset> GetAsset(AssetHandle handle) override;
	public:
		[[nodiscard]] const AssetMetadata& GetMetadata(AssetHandle handle) const;
		Ref<Asset> ImportAsset(Path assetPath);
		Ref<Asset> GetOrCreateAsset(Path assetPath);

		[[nodiscard]] Path GetFilePath(AssetHandle) const;
		[[nodiscard]] AssetMetadata GetMetadata(const Path&) const;

		void SetPath(AssetHandle handle, Path newPath);

		bool AddAsset(Ref<Asset> asset);
		bool AddAsset(Ref<Asset> asset, Path path);

		void RemoveAsset(AssetHandle handle);
		void UnloadAsset(AssetHandle handle);

		void SerializeAssetRegistry();
		void DeserializeAssetRegistry();
	public:
		template<typename T, typename ...Args>
		Ref<T> CreateAsset(Args&&... args);

		template<typename T, typename ...Args>
		Ref<T> CreateAsset(Path path, Args&&... args);
	private:
		AssetMap m_LoadedAssets;
		AssetRegistry m_AssetRegistry;

		//TODO: Memory-only assets;
	};


	template<typename T, typename... Args>
	Ref<T> EditorAssetManager::CreateAsset(Args &&...args)
	{
		AssetMetadata metadata;
		metadata.FilePath = {FileSource::Cache, "MemoryAssets/"};
		metadata.FilePath.path += metadata.Handle.string() + ".vxm_memory";
		metadata.Type = T::GetStaticType();
		Ref<T> asset = CreateRef<T>(std::forward<Args>(args)...);
		asset->Handle = metadata.Handle;
		m_AssetRegistry.emplace(metadata.Handle, metadata);
		m_LoadedAssets.emplace(metadata.Handle, asset);
		SerializeAssetRegistry();
		return asset;
	}

	template<typename T, typename ...Args>
	inline Ref<T> EditorAssetManager::CreateAsset(Path path, Args&&... args)
	{
		AssetMetadata metadata;
		metadata.FilePath = path;
		metadata.Type = T::GetStaticType();
		Ref<T> asset = CreateRef<T>(std::forward<Args>(args)...);
		asset->Handle = metadata.Handle;
		m_AssetRegistry.emplace(metadata.Handle, metadata);
		m_LoadedAssets.emplace(metadata.Handle, asset);
		SerializeAssetRegistry();
		return asset;
	}

} // namespace Voxymore::Core

